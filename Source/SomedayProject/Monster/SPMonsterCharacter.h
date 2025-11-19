#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "SomedayProjectCharacter.h"
#include "SPCommonDefines.h"
#include "SPMonsterCharacter.generated.h"

class UAbilitySystemComponent;
class USPAbilitySystemComponent;
class USPBaseAttributeSet;
class USPMonsterDataAsset;
class UGameplayAbility;
class UGameplayEffect;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class SOMEDAYPROJECT_API ASPMonsterCharacter : public ASomedayProjectCharacter
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

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<USPBaseAttributeSet> MonsterAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackRetryDistance = 75.0f;

protected:
	void ResetCharacter () override;
	void InitializeAttributes() override;
	void GrantAbilities();

	//AI
	UFUNCTION()
	void HandlePawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void InitAISetting();

	//Lifecycle
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	void HandleDeath();
	

private:
	float LastAttackTime = -1000.0f;
	EObjectState MonsterState;

	FGameplayAbilitySpecHandle GrantedAttackAbilityHandle;
};


