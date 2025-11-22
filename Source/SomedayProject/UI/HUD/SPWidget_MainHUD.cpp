// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SPWidget_MainHUD.h"
#include "UI/HUD/SPWidget_HPBar.h"
#include "Hero/SPHeroComponent.h"

void USPWidget_MainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	HealthBarWidget = Cast<USPWidget_HPBar>(GetWidgetFromName(TEXT("HPBarWidget")));

	if (USPHeroComponent* HeroComponent = OwningPawn->FindComponentByClass<USPHeroComponent>())
	{
		if (HealthBarWidget)
		{
			HeroComponent->OnHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnHealthChanged);
			HeroComponent->OnMaxHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnMaxHealthChanged);
		}
	}
}
