#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SPMonsterDataAsset.generated.h"

class UGameplayAbility;
class UAnimMontage;
class UBehaviorTree;

UCLASS(BlueprintType)
class SOMEDAYPROJECT_API USPMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USPMonsterDataAsset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	FString MonsterID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	float MoveSpeed = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	float DetectionRadius = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	float LoseSightRadius = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	float AttackRadius = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|Data")
	float AttackInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPAbilitySystem")
	TSubclassOf<UGameplayAbility> AttackAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|AnimMontage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|AnimMontage")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPMonster|UI")
	TSubclassOf<UUserWidget> TopWidget;
};


