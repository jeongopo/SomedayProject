#include "Monster/SPGameAbility_MonsterAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "Monster/SPMonsterCharacter.h"
#include "Core/SPBaseAttributeSet.h"
#include "Monster/SPMonsterDataAsset.h"
#include "SPLogHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagContainer.h"

USPGameAbility_MonsterAttack::USPGameAbility_MonsterAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USPGameAbility_MonsterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	ASPMonsterCharacter* MonsterCharacter = Cast<ASPMonsterCharacter>(AvatarActor);
	if (!MonsterCharacter)
	{
		LOG_WARNING(LogSPDefault, TEXT("USPGameAbility_MonsterAttack::ActivateAbility - MonsterCharacter is null"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* TargetActor = MonsterCharacter->GetCurrentTarget();
	if (!TargetActor)
	{
		// Fallback: target nearest player
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				TargetActor = PlayerController->GetPawn();
			}
		}
	}

	USPMonsterDataAsset* MonsterData = MonsterCharacter->GetMonsterData();
	if (!TargetActor || !MonsterData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const float EffectiveRange = AttackRangeOverride > 0.0f ? AttackRangeOverride : MonsterData->AttackRange;
	const float EffectiveRadius = AttackRadiusOverride > 0.0f ? AttackRadiusOverride : MonsterData->AttackRadius;
	const float DamageAmount = FMath::Max(MonsterData->AttackPower, BaseDamage);

	if (!IsTargetWithinCone(MonsterCharacter, TargetActor, EffectiveRange))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (MonsterData->AttackMontage)
	{
		if (UAnimInstance* AnimInstance = MonsterCharacter->GetMesh() ? MonsterCharacter->GetMesh()->GetAnimInstance() : nullptr)
		{
			AnimInstance->Montage_Play(MonsterData->AttackMontage);
		}
	}

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);
	ActorsToIgnore.Add(MonsterCharacter);

	const FVector StartLocation = AvatarActor->GetActorLocation();
	const FVector ForwardVector = AvatarActor->GetActorForwardVector();
	const FVector EndLocation = StartLocation + (ForwardVector * EffectiveRange);

	UKismetSystemLibrary::SphereTraceMulti(
		AvatarActor,
		StartLocation,
		EndLocation,
		EffectiveRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);

	if (HitResults.Num() == 0)
	{
		// Attempt direct application to the target actor even if trace missed
		ApplyDamageToTarget(AvatarActor, TargetActor, FHitResult(), DamageAmount);
	}
	else
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!HitActor)
			{
				continue;
			}

			ApplyDamageToTarget(AvatarActor, HitActor, HitResult, DamageAmount);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void USPGameAbility_MonsterAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool USPGameAbility_MonsterAttack::IsTargetWithinCone(AActor* InstigatorActor, AActor* TargetActor, float EffectiveRange) const
{
	if (!InstigatorActor || !TargetActor)
	{
		return false;
	}

	const FVector ToTarget = TargetActor->GetActorLocation() - InstigatorActor->GetActorLocation();
	const float DistanceToTarget = ToTarget.Length();

	if (DistanceToTarget > EffectiveRange)
	{
		return false;
	}

	const FVector ForwardVector = InstigatorActor->GetActorForwardVector();
	const FVector Direction = ToTarget.GetSafeNormal();
	const float DotProduct = FVector::DotProduct(ForwardVector, Direction);
	const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f)));

	return AngleDegrees <= (AttackConeAngle * 0.5f);
}

void USPGameAbility_MonsterAttack::ApplyDamageToTarget(AActor* InstigatorActor, AActor* TargetActor, const FHitResult& HitResult, float DamageAmount) const
{
	if (!InstigatorActor || !TargetActor)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	const ASPMonsterCharacter* MonsterCharacter = Cast<ASPMonsterCharacter>(InstigatorActor);
	if (!MonsterCharacter)
	{
		return;
	}

	TSubclassOf<UGameplayEffect> EffectClass = DamageEffectClass;
	if (!EffectClass)
	{
		EffectClass = MonsterCharacter->GetDamageEffectClass();
	}

	if (!EffectClass)
	{
		LOG_WARNING(LogSPDefault, TEXT("USPGameAbility_MonsterAttack::ApplyDamageToTarget - DamageEffectClass not set"));
		return;
	}

	UAbilitySystemComponent* InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);
	if (!InstigatorASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = InstigatorASC->MakeEffectContext();
	EffectContext.AddHitResult(HitResult);
	EffectContext.AddInstigator(InstigatorActor, InstigatorActor);

	FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), EffectContext);
	if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
	{
		static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"), false);
		if (DamageTag.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, DamageAmount);
		}

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}


