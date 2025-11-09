// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/SPGameplayAbility.h"
#include "SPGameAbility_WeaponDefault.generated.h"

class ACharacter;
class UAnimMontage;
class UGameplayEffect;
class USkeletalMeshComponent;
class AActor;

UCLASS()
class SOMEDAYPROJECT_API USPGameAbility_WeaponDefault : public USPGameplayAbility
{
	GENERATED_BODY()

public:
	USPGameAbility_WeaponDefault();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 공격 범위 (센티미터)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float AttackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float AttackAngle = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float CollisionCheckDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float CollisionCheckDuration = 0.3f;

private:
	void PlayAttackAnimation();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void StartCollisionCheck();
	void StopCollisionCheck();

	void OnAbilityEndTimer();

	void CheckForHits();
	void ApplyDamageToTarget(AActor* Target, const FHitResult& HitResult);

	UPROPERTY()
	TSet<AActor*> HitTargets;

	FTimerHandle CollisionCheckTimerHandle;
	FTimerHandle CollisionCheckStopTimerHandle;
	FTimerHandle AbilityEndTimerHandle;
};
