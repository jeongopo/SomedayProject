// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPWidget_HPBar.generated.h"

class UProgressBar;
class USPAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPWidget_HPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized () override;

	void RefreshWidget();

	//delegate
	UFUNCTION()
	void OnHealthChanged (const USPAbilitySystemComponent* InHeroComp, float OldValue, float NewValue);

	UFUNCTION()
	void OnMaxHealthChanged (const USPAbilitySystemComponent* InHeroComp, float OldValue, float NewValue);
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	float CurrentValue;
	float MaxValue;
};
