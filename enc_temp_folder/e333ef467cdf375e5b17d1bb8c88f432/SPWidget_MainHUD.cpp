// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SPWidget_MainHUD.h"
#include "UI/HUD/SPWidget_HPBar.h"
#include "Hero/SPHeroComponent.h"
#include "SomedayProjectCharacter.h"
#include "Core/SPBaseAttributeSet.h"

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
		HeroComponent->OnHealthChangedDelegate.AddDynamic(this, &USPWidget_MainHUD::OnHealthChanged);
	}
	
	if (ASomedayProjectCharacter* SPCharacter = Cast<ASomedayProjectCharacter>(OwningPawn))
	{
		if (USPAbilitySystemComponent * AbilitySysComp = SPCharacter->GetSPAbilitySystemComponent())
		{
			if (HealthBarWidget)
			{
				float MaxValue = AbilitySysComp->GetNumericAttribute(USPBaseAttributeSet::GetMaxHealthAttribute());
				float CurrentValue = AbilitySysComp->GetNumericAttribute(USPBaseAttributeSet::GetHealthAttribute());
				HealthBarWidget->InitProgressData(CurrentValue, MaxValue);
			}
		}
	}
}

void USPWidget_MainHUD::OnHealthChanged(const USPHeroComponent* InHeroComp, float OldValue, float NewValue)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->OnHealthChanged(OldValue, NewValue);
	}
}
