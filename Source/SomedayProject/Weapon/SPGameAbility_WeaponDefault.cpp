// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/SPGameAbility_WeaponDefault.h"
#include "SPLogHelper.h"
#include "SomedayProjectCharacter.h"
#include "Weapon/WeaponManagerComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

USPGameAbility_WeaponDefault::USPGameAbility_WeaponDefault()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USPGameAbility_WeaponDefault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	LOG_INFO(LogSPDefault, TEXT("USPGameAbility_WeaponDefault::ActivateAbility"));

	HitTargets.Empty();
	PlayAttackAnimation();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			CollisionCheckTimerHandle,
			this,
			&USPGameAbility_WeaponDefault::StartCollisionCheck,
			CollisionCheckDelay,
			false
		);
	}

	if (!AttackMontage)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				AbilityEndTimerHandle,
				this,
				&USPGameAbility_WeaponDefault::OnAbilityEndTimer,
				CollisionCheckDelay + CollisionCheckDuration + 0.1f,
				false
			);
		}
	}
}

void USPGameAbility_WeaponDefault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CollisionCheckTimerHandle);
		World->GetTimerManager().ClearTimer(CollisionCheckStopTimerHandle);
		World->GetTimerManager().ClearTimer(AbilityEndTimerHandle);
	}

	HitTargets.Empty();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USPGameAbility_WeaponDefault::PlayAttackAnimation()
{
	if (!AttackMontage)
	{
		LOG_WARNING(LogSPDefault, TEXT("AttackMontage is not set!"));
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AvatarActor);
	if (!Character)
	{
		return;
	}

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->Montage_Play(AttackMontage, 1.0f))
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &USPGameAbility_WeaponDefault::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
	}
}

void USPGameAbility_WeaponDefault::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage && !bInterrupted)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void USPGameAbility_WeaponDefault::StartCollisionCheck()
{
	CheckForHits();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			CollisionCheckTimerHandle,
			this,
			&USPGameAbility_WeaponDefault::CheckForHits,
			0.05f,
			true
		);

		World->GetTimerManager().SetTimer(
			CollisionCheckStopTimerHandle,
			this,
			&USPGameAbility_WeaponDefault::StopCollisionCheck,
			CollisionCheckDuration,
			false
		);
	}
}

void USPGameAbility_WeaponDefault::StopCollisionCheck()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CollisionCheckTimerHandle);
	}
}

void USPGameAbility_WeaponDefault::OnAbilityEndTimer()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USPGameAbility_WeaponDefault::CheckForHits()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AvatarActor);
	if (!Character)
	{
		return;
	}

	FVector CharacterLocation = Character->GetActorLocation();
	FVector CharacterForward = Character->GetActorForwardVector();

	FVector AttackStartLocation = CharacterLocation;
	AActor* WeaponActor = nullptr;

	if (ASomedayProjectCharacter* SPCharacter = Cast<ASomedayProjectCharacter>(Character))
	{
		if (UWeaponManagerComponent* WeaponManager = SPCharacter->GetComponentByClass<UWeaponManagerComponent>())
		{
			WeaponActor = WeaponManager->GetFirstEquippedWeapon();
			if (WeaponActor)
			{
				AttackStartLocation = WeaponActor->GetActorLocation();
			}
		}
	}
	FVector AttackEndLocation = AttackStartLocation + (CharacterForward * AttackRange);

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);
	if (WeaponActor)
	{
		ActorsToIgnore.Add(WeaponActor);
	}

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		AttackStartLocation,
		AttackEndLocation,
		50.0f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, 
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			continue;
		}

		if (HitTargets.Contains(HitActor))
		{
			continue;
		}

		FVector ToTarget = (HitActor->GetActorLocation() - CharacterLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(CharacterForward, ToTarget);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

		if (Angle <= AttackAngle / 2.0f)
		{
			ApplyDamageToTarget(HitActor, Hit);
			HitTargets.Add(HitActor);
		}
	}
}

void USPGameAbility_WeaponDefault::ApplyDamageToTarget(AActor* Target, const FHitResult& HitResult)
{
	if (!Target)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!TargetASC)
	{
		return;
	}

	if (DamageEffectClass)
	{
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
		
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		TargetData->HitResult = HitResult;
		TargetDataHandle.Add(TargetData);

		/*
		// EffectContext에 TargetData 추가
		if (FGameplayEffectContext* EffectContext = EffectContextHandle.Get())
		{
			EffectContext->AddTargetData(TargetDataHandle);
		}
		*/

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

		if (EffectSpecHandle.IsValid() && EffectSpecHandle.Data.IsValid())
		{
			EffectSpecHandle.Data->SetContext(EffectContextHandle);
			FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			if (ActiveEffectHandle.IsValid())
			{
				LOG_INFO(LogSPDefault, TEXT("Damage applied to target: %s"), *Target->GetName());
			}
		}
	}
	else
	{
		LOG_WARNING(LogSPDefault, TEXT("DamageEffectClass is not set!"));
	}
}
