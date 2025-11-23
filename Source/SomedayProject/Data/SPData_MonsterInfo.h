// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPData_MonsterInfo.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct SOMEDAYPROJECT_API FSPDataRow_MonsterInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AttackPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Defense;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> ItemList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MonsterDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MonsterIconPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MonsterModelPath;
};

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPData_MonsterInfo : public UDataTable
{
	GENERATED_BODY()
	
};
