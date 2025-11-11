// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/SPGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SPGameAbility_WeaponDefault.generated.h"

class ACharacter;
class UAnimMontage;
class UGameplayEffect;
class USkeletalMeshComponent;
class AActor;
class UAbilitySystemComponent;
struct FGameplayEventData;

UCLASS()
class SOMEDAYPROJECT_API USPGameAbility_WeaponDefault : public USPGameplayAbility
{
	GENERATED_BODY()

public:
	USPGameAbility_WeaponDefault();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
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

	//충돌처리 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float AttackDuration = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.01"))
	float CollisionCheckInterval = 0.05f;

private:
	void PlayAttackAnimation();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void StartCollisionCheck();
	void StopCollisionCheck();

	void CheckForHits();
	void ApplyDamageToTarget(AActor* Target, const FHitResult& HitResult);

	// GameplayEvent 핸들러 (노티파이에서 호출됨)
	void HandleAttackStartEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

private:
	UPROPERTY()
	TSet<AActor*> HitTargets;

	FTimerHandle CollisionCheckTimerHandle;
	FTimerHandle CollisionCheckStopTimerHandle;
	FTimerHandle AbilityEndTimerHandle;

	// GameplayEvent 바인딩 핸들
	FDelegateHandle AttackStartEventHandle;
	FGameplayTag AttackStartTag;
};
