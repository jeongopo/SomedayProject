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

	// 공격 충돌 체크 지속 시간 (노티파이 받은 후, 초)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0"))
	float AttackDuration = 0.3f;

	// 충돌 체크 주기 (초)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.01"))
	float CollisionCheckInterval = 0.05f;

	// 레거시 지원 (노티파이가 없을 경우를 위한 폴백)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Legacy", meta = (ClampMin = "0.0"))
	float CollisionCheckDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Legacy", meta = (ClampMin = "0.0"))
	float CollisionCheckDuration = 0.3f;

private:
	void PlayAttackAnimation();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void StartCollisionCheck();
	void StopCollisionCheck();

	void OnAbilityEndTimer();

	void CheckForHits();
	void ApplyDamageToTarget(AActor* Target, const FHitResult& HitResult);

	// GameplayEvent 핸들러 (노티파이에서 호출됨)
	void HandleAttackStartEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	UPROPERTY()
	TSet<AActor*> HitTargets;

	FTimerHandle CollisionCheckTimerHandle;
	FTimerHandle CollisionCheckStopTimerHandle;
	FTimerHandle AbilityEndTimerHandle;

	// GameplayEvent 바인딩 핸들
	FDelegateHandle AttackStartEventHandle;
	FGameplayTag AttackStartTag;
};
