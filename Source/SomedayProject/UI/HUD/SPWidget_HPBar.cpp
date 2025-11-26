// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SPWidget_HPBar.h"
#include "Components/ProgressBar.h"

void USPWidget_HPBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
}

void USPWidget_HPBar::OnHealthChanged(const USPAbilitySystemComponent* InHeroComp, float OldValue, float NewValue)
{
	CurrentValue = NewValue;
	RefreshWidget();
}

void USPWidget_HPBar::OnMaxHealthChanged(const USPAbilitySystemComponent* InHeroComp, float OldValue, float NewValue)
{
	MaxValue = NewValue;
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
