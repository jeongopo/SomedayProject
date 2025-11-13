#pragma once

#include "CoreMinimal.h"
#include "Ability/SPGameplayAbility.h"
#include "SPGameAbility_MonsterAttack.generated.h"

class UAnimMontage;
class UGameplayEffect;

UCLASS()
class SOMEDAYPROJECT_API USPGameAbility_MonsterAttack : public USPGameplayAbility
{
	GENERATED_BODY()

public:
	USPGameAbility_MonsterAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackConeAngle = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRangeOverride = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRadiusOverride = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

private:
	void ApplyDamageToTarget(AActor* InstigatorActor, AActor* TargetActor, const FHitResult& HitResult, float DamageAmount) const;

	bool IsTargetWithinCone(AActor* InstigatorActor, AActor* TargetActor, float EffectiveRange) const;
};


