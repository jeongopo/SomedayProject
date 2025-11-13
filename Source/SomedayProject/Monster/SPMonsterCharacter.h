#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "SPMonsterCharacter.generated.h"

class UAbilitySystemComponent;
class USPAbilitySystemComponent;
class USPMonsterAttributeSet;
class USPMonsterDataAsset;
class UGameplayAbility;
class UGameplayEffect;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class SOMEDAYPROJECT_API ASPMonsterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASPMonsterCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetCurrentTarget() const { return CurrentTarget.Get(); }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetAttackRange() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetAttackRadius() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetAttackPower() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	TSubclassOf<UGameplayEffect> GetDamageEffectClass() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	USPMonsterDataAsset* GetMonsterData() const { return MonsterData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<USPMonsterDataAsset> MonsterData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<USPAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<USPMonsterAttributeSet> MonsterAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackRetryDistance = 75.0f;

	void InitializeAttributes();
	void GrantAbilities();

	UFUNCTION()
	void HandlePawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	void HandleDeath();
	void UpdateMovementTowardsTarget(float DeltaSeconds);
	void TryPerformAttack();

private:
	float LastAttackTime = -1000.0f;

	FGameplayAbilitySpecHandle GrantedAttackAbilityHandle;
};


