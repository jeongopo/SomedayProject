// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPWidget_HPBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPWidget_HPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeOnInitialized () override;
	
	void InitProgressData (float InCurrentValue, float InMaxValue);
	void OnHealthChanged (float OldValue, float NewValue);

	void RefreshWidget ();

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	float CurrentValue;
	float MaxValue;
};
