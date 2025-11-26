// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPUIDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SOMEDAYPROJECT_API FSPUIDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WidgetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ZOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowByDefault;
};
