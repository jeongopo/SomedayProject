// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPWidget_MainHUD.generated.h"

class USPWidget_HPBar;
class USPHeroComponent;

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPWidget_MainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized () override;

	UFUNCTION()
	void OnHealthChanged (const USPHeroComponent* InHeroComp, float OldValue, float NewValue);

protected:
	UPROPERTY()
	USPWidget_HPBar* HealthBarWidget;
};
