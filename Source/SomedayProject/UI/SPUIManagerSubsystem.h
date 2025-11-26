// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPUIManagerSubsystem.generated.h"

class UUserWidget;
class USPUIDataTable;

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	USPUIManagerSubsystem();

	void Initialize (FSubsystemCollectionBase& Collection) override;

public:
	void InitUIData ();

protected:
	UPROPERTY()
	TMap<FName, UUserWidget*> WidgetMap;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UDataTable> UIDataTable;
};
