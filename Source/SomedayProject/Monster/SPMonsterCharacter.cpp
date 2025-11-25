#include "Monster/SPMonsterCharacter.h"

#include "AIController.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#include "Core/SPAbilitySystemComponent.h"
#include "Core/SPBaseAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/SPMonsterDataAsset.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SPLogHelper.h"

#include "UI/Gameplay/SPActorWidget_MonsterTop.h"
#include "Data/SPDataManagerSubsystem.h"
#include "Data/SPData_MonsterInfo.h"

ASPMonsterCharacter::ASPMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	MonsterAttributes = CreateDefaultSubobject<USPBaseAttributeSet>(TEXT("MonsterAttributes"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
}

void ASPMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USPBaseAttributeSet::GetHealthAttribute()).AddUObject(this, &ASPMonsterCharacter::HandleHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USPBaseAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ASPMonsterCharacter::HandleMaxHealthChanged);
	}

	if (MonsterData)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = MonsterData->MoveSpeed;
		}

		if (WidgetComponent && MonsterData->TopWidget)
		{
			WidgetComponent->SetWidgetClass(MonsterData->TopWidget);
		}

		if (PerceptionComponent)
		{
			UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>(this, UAISenseConfig_Sight::StaticClass());

			if (SightConfig)
			{
				SightConfig->SightRadius = 1000.0f;
				SightConfig->LoseSightRadius = 1200.0f;
				SightConfig->PeripheralVisionAngleDegrees = 70.0f;
				SightConfig->DetectionByAffiliation.bDetectEnemies = true;
				SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
				SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

				PerceptionComponent->ConfigureSense(*SightConfig);
				PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
			}

			PerceptionComponent->RequestStimuliListenerUpdate();
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASPMonsterCharacter::HandleTargetPerceptionUpdated);
		}

		if (MonsterData->BehaviorTree)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->RunBehaviorTree(MonsterData->BehaviorTree);

				if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
				{
					Blackboard->SetValueAsVector("OriginPosition", GetActorLocation());
				}
			}
		}
	}
	
	USPDataManagerSubsystem* DataManager = GetGameInstance()->GetSubsystem<USPDataManagerSubsystem>();
	if (!DataManager)
	{
		return;
	}

	//쓰는 구간이 많아서 먼저 캐싱
	CachedMonsterInfo = DataManager->FindDataRowFromDataTable<FSPDataRow_MonsterInfo>(ESPDataTableType::MonsterData, MonsterData->MonsterID);

	InitializeWidget();
	InitializeAttributes();
	GrantAbilities();
}

void ASPMonsterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UAbilitySystemComponent* ASPMonsterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TSubclassOf<UGameplayEffect> ASPMonsterCharacter::GetDamageEffectClass() const
{
	return MonsterData ? MonsterData->DamageEffectClass : nullptr;
}

void ASPMonsterCharacter::ResetCharacter()
{
	CharacterState = EObjectState::Idle;
	InitAISetting();
}

void ASPMonsterCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent || !MonsterAttributes || !MonsterData)
	{
		return;
	}

	const float MaxHealthValue = CachedMonsterInfo ? CachedMonsterInfo->MaxHealth : MonsterAttributes->GetMaxHealth();
	const float AttackPowerValue = CachedMonsterInfo ? CachedMonsterInfo->AttackPower : MonsterAttributes->GetAttackPower();
	const float DefenseValue = CachedMonsterInfo ? CachedMonsterInfo->Defense : MonsterAttributes->GetDefense();
	const float AttackRangeValue = CachedMonsterInfo ? CachedMonsterInfo->AttackRange : MonsterAttributes->GetAttackRange();

	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetMaxHealthAttribute(), MaxHealthValue);
	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetHealthAttribute(), MaxHealthValue);
	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetAttackPowerAttribute(), AttackPowerValue);
	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetDefenseAttribute(), DefenseValue);
	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetAttackRangeAttribute(), AttackRangeValue);
}

void ASPMonsterCharacter::GrantAbilities()
{
	if (!AbilitySystemComponent || !MonsterData)
	{
		return;
	}

	if (!ACharacter::HasAuthority())
	{
		return;
	}

	if (MonsterData->AttackAbilityClass)
	{
		const int32 AbilityLevel = 1;
		FGameplayAbilitySpec AbilitySpec(MonsterData->AttackAbilityClass, AbilityLevel);
		AbilitySpec.SourceObject = this;
		GrantedAttackAbilityHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void ASPMonsterCharacter::InitializeWidget()
{
	USPActorWidget_MonsterTop* MonsterWidget = GetAttachedWidget<USPActorWidget_MonsterTop>();
	if (MonsterWidget == nullptr)
	{
		return;
	}

	MonsterWidget->InitializedWithAbilitySystem(GetSPAbilitySystemComponent());
}

void ASPMonsterCharacter::HandlePawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn || SeenPawn == this)
	{
		return;
	}

	CurrentTarget = SeenPawn;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->SetFocus(SeenPawn);
	}
}

void ASPMonsterCharacter::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (APawn* SensedPawn = Cast<APawn>(Actor))
		{
			HandlePawnSeen(SensedPawn);
			Blackboard->SetValueAsObject(TEXT("TargetActor"), SensedPawn);
			CharacterState = EObjectState::Attacking;
		}
	}
	else
	{
		if (CurrentTarget.Get() == Actor)
		{
			InitAISetting();
		}
	}
}

void ASPMonsterCharacter::InitAISetting ()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	CurrentTarget.Reset();
	Blackboard->ClearValue(TEXT("TargetActor"));
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	AIController->StopMovement();
}

void ASPMonsterCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	UE_LOG(LogSPDefault, Log, TEXT("Monster Health Changed %f -> %f"), ChangeData.OldValue, ChangeData.NewValue);

	const float NewValue = ChangeData.NewValue;
	if (NewValue <= 0.0f)
	{
		HandleDeath();
	}

	OnHealthChangedDelegate.Broadcast(GetSPAbilitySystemComponent(), ChangeData.OldValue, ChangeData.NewValue);
}

void ASPMonsterCharacter::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChangedDelegate.Broadcast(GetSPAbilitySystemComponent(), ChangeData.OldValue, ChangeData.NewValue);
}

void ASPMonsterCharacter::HandleDeath()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	// TODO: add death animation / drop logic / experience reward hook
	ACharacter::Destroy();
}


