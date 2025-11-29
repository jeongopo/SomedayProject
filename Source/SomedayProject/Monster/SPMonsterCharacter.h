#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "SomedayProjectCharacter.h"
#include "SPCommonDefines.h"
#include "Hero/SPHeroComponent.h"
#include "Data/SPData_MonsterInfo.h"
#include "SPMonsterCharacter.generated.h"

class UAbilitySystemComponent;
class USPAbilitySystemComponent;
class UWidgetComponent;
class UAIPerceptionComponent;
class USPBaseAttributeSet;
class USPMonsterDataAsset;
class UGameplayAbility;
class UGameplayEffect;
class UAISenseConfig_Sight;
struct FSPDataRow_MonsterInfo;

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

	UFUNCTION(BlueprintCallable, Category = "AI")
	USPMonsterDataAsset* GetMonsterData() const { return MonsterData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<USPMonsterDataAsset> MonsterData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<USPBaseAttributeSet> MonsterAttributes;

public:
	FSPHealth_AttributeChanged OnHealthChangedDelegate;
	FSPHealth_AttributeChanged OnMaxHealthChangedDelegate;

protected:
	void ResetCharacter () override;
	void InitializeAttributes() override;
	void GrantAbilities();

	void InitializeWidget () override;
	void OnDamaged (ESP_AttackType InAttackType, ESP_AbnormalType InAbnormalType) override;

	//AI
	UFUNCTION()
	void HandlePawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void InitAISetting();

	//Lifecycle
	void HandleHealthChanged (const FOnAttributeChangeData& ChangeData);
	void HandleMaxHealthChanged (const FOnAttributeChangeData& ChangeData);
	void HandleDeath();

public:
	const FSPDataRow_MonsterInfo* GetCachedMonsterInfo() { return CachedMonsterInfo; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TWeakObjectPtr<AActor> CurrentTarget;

	FSPDataRow_MonsterInfo* CachedMonsterInfo;

private:
	FGameplayAbilitySpecHandle GrantedAttackAbilityHandle;
};


