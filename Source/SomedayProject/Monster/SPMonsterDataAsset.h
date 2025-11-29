#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SPMonsterDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAnimMontage;
class UBehaviorTree;



UCLASS(BlueprintType)
class SOMEDAYPROJECT_API USPMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USPMonsterDataAsset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	FString MonsterID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	float MoveSpeed = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	float DetectionRadius = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	float LoseSightRadius = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	float AttackRadius = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	float AttackInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPAbilitySystem")
	TSubclassOf<UGameplayAbility> AttackAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPAbilitySystem")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPAbilitySystem")
	TObjectPtr<UAnimMontage> HitMontage;

	//°ð ¾È¾¸
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Value")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|UI")
	TSubclassOf<UUserWidget> TopWidget;
};


