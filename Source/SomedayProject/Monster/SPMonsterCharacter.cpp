#include "Monster/SPMonsterCharacter.h"

#include "AIController.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#include "Core/SPAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/SPMonsterAttributeSet.h"
#include "Monster/SPMonsterDataAsset.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

ASPMonsterCharacter::ASPMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<USPAbilitySystemComponent>(TEXT("MonsterAbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	MonsterAttributes = CreateDefaultSubobject<USPMonsterAttributeSet>(TEXT("MonsterAttributes"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (PerceptionComponent && SightConfig)
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
}

void ASPMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);

		AbilitySystem->GetGameplayAttributeValueChangeDelegate(USPMonsterAttributeSet::GetHealthAttribute()).AddUObject(this, &ASPMonsterCharacter::HandleHealthChanged);
	}

	if (MonsterData && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MonsterData->MoveSpeed;
	}

	if (MonsterData && SightConfig)
	{
		SightConfig->SightRadius = MonsterData->DetectionRadius;
		SightConfig->LoseSightRadius = MonsterData->LoseSightRadius;
		if (PerceptionComponent)
		{
			PerceptionComponent->ConfigureSense(*SightConfig);
			PerceptionComponent->RequestStimuliListenerUpdate();
		}
	}

	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASPMonsterCharacter::HandleTargetPerceptionUpdated);
	}

	InitializeAttributes();
	GrantAbilities();
}

void ASPMonsterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateMovementTowardsTarget(DeltaSeconds);
	TryPerformAttack();
}

UAbilitySystemComponent* ASPMonsterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

float ASPMonsterCharacter::GetAttackRange() const
{
	if (MonsterAttributes)
	{
		return MonsterAttributes->GetAttackRange();
	}

	return MonsterData ? MonsterData->AttackRange : 200.0f;
}

float ASPMonsterCharacter::GetAttackRadius() const
{
	if (MonsterData)
	{
		return MonsterData->AttackRadius;
	}

	return 75.0f;
}

float ASPMonsterCharacter::GetAttackPower() const
{
	if (MonsterAttributes)
	{
		return MonsterAttributes->GetAttackPower();
	}
	return MonsterData ? MonsterData->AttackPower : 10.0f;
}

TSubclassOf<UGameplayEffect> ASPMonsterCharacter::GetDamageEffectClass() const
{
	return MonsterData ? MonsterData->DamageEffectClass : nullptr;
}

void ASPMonsterCharacter::InitializeAttributes()
{
	if (!AbilitySystem || !MonsterAttributes)
	{
		return;
	}

	const float MaxHealthValue = MonsterData ? MonsterData->MaxHealth : MonsterAttributes->GetMaxHealth();
	const float AttackPowerValue = MonsterData ? MonsterData->AttackPower : MonsterAttributes->GetAttackPower();
	const float DefenseValue = MonsterData ? MonsterData->Defense : MonsterAttributes->GetDefense();
	const float AttackRangeValue = MonsterData ? MonsterData->AttackRange : MonsterAttributes->GetAttackRange();

	AbilitySystem->SetNumericAttributeBase(USPMonsterAttributeSet::GetMaxHealthAttribute(), MaxHealthValue);
	AbilitySystem->SetNumericAttributeBase(USPMonsterAttributeSet::GetHealthAttribute(), MaxHealthValue);
	AbilitySystem->SetNumericAttributeBase(USPMonsterAttributeSet::GetAttackPowerAttribute(), AttackPowerValue);
	AbilitySystem->SetNumericAttributeBase(USPMonsterAttributeSet::GetDefenseAttribute(), DefenseValue);
	AbilitySystem->SetNumericAttributeBase(USPMonsterAttributeSet::GetAttackRangeAttribute(), AttackRangeValue);
}

void ASPMonsterCharacter::GrantAbilities()
{
	if (!AbilitySystem || !MonsterData)
	{
		return;
	}

	if (!HasAuthority())
	{
		return;
	}

	if (MonsterData->AttackAbilityClass)
	{
		const int32 AbilityLevel = 1;
		FGameplayAbilitySpec AbilitySpec(MonsterData->AttackAbilityClass, AbilityLevel);
		AbilitySpec.SourceObject = this;
		GrantedAttackAbilityHandle = AbilitySystem->GiveAbility(AbilitySpec);
	}
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

	if (Stimulus.WasSuccessfullySensed())
	{
		if (APawn* SensedPawn = Cast<APawn>(Actor))
		{
			HandlePawnSeen(SensedPawn);
		}
	}
	else
	{
		if (CurrentTarget.Get() == Actor)
		{
			CurrentTarget.Reset();

			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				AIController->ClearFocus(EAIFocusPriority::Gameplay);
				AIController->StopMovement();
			}
		}
	}
}

void ASPMonsterCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	const float NewValue = ChangeData.NewValue;
	if (NewValue <= 0.0f)
	{
		HandleDeath();
	}
}

void ASPMonsterCharacter::HandleDeath()
{
	if (AbilitySystem)
	{
		AbilitySystem->CancelAllAbilities();
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	// TODO: add death animation / drop logic / experience reward hook
	Destroy();
}

void ASPMonsterCharacter::UpdateMovementTowardsTarget(float DeltaSeconds)
{
	if (!CurrentTarget.IsValid())
	{
		return;
	}

	AActor* TargetActor = CurrentTarget.Get();
	if (!TargetActor)
	{
		CurrentTarget.Reset();
		return;
	}

	const float DistanceSq = FVector::DistSquared(TargetActor->GetActorLocation(), GetActorLocation());
	const float DesiredRange = FMath::Square(FMath::Max(GetAttackRange() - AttackRetryDistance, 0.0f));

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	if (DistanceSq > DesiredRange)
	{
		AIController->MoveToActor(TargetActor, AttackRetryDistance, true, true, true, nullptr, true);
	}
	else
	{
		AIController->StopMovement();
	}
}

void ASPMonsterCharacter::TryPerformAttack()
{
	if (!AbilitySystem || !MonsterData || !CurrentTarget.IsValid())
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < MonsterData->AttackInterval)
	{
		return;
	}

	const AActor* TargetActor = CurrentTarget.Get();
	const float Distance = TargetActor ? FVector::Dist(TargetActor->GetActorLocation(), GetActorLocation()) : MAX_flt;

	if (Distance <= GetAttackRange())
	{
		if (MonsterData->AttackAbilityClass)
		{
			if (AbilitySystem->TryActivateAbilityByClass(MonsterData->AttackAbilityClass))
			{
				LastAttackTime = CurrentTime;
			}
		}
	}
}


