// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SPWidget_HPBar.h"
#include "Components/ProgressBar.h"

void USPWidget_HPBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
}

void USPWidget_HPBar::InitProgressData(float InCurrentValue, float InMaxValue)
{
	MaxValue = InMaxValue;
	CurrentValue = InCurrentValue;

	RefreshWidget();
}

void USPWidget_HPBar::OnHealthChanged(float OldValue, float NewValue)
{
	CurrentValue = NewValue;
	RefreshWidget();
}

void USPWidget_HPBar::RefreshWidget()
{
	if (ProgressBar)
	{
		float Ratio = MaxValue == 0 ? 0.0f : CurrentValue / MaxValue;
		ProgressBar->SetPercent(Ratio);
	}
}
