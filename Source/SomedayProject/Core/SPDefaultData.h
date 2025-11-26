// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "SPDefaultData.generated.h"

class UInputAction;
class UUserWidget;

USTRUCT(BlueprintType)
struct FSPInputActionBindData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};


/**
 * 
 */
UCLASS(BlueprintType)
class SOMEDAYPROJECT_API USPDefaultData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SPHero|Input")
	TArray<FSPInputActionBindData> InputActionBindData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SPHero|Attributes")
	float MaxHealth = 150.0f;
};
