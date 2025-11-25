// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/SPActorWidget_MonsterTop.h"
#include "UI/HUD/SPWidget_HPBar.h"
#include "Components/TextBlock.h"

#include "Monster/SPMonsterCharacter.h"
#include "Core/SPAbilitySystemComponent.h"
#include "Core/SPBaseAttributeSet.h"
#include "Data/SPData_MonsterInfo.h"

void USPActorWidget_MonsterTop::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPActorWidget_MonsterTop::InitializedWithAbilitySystem(USPAbilitySystemComponent* InASC)
{
	//NativeOnInitialized 보다 늦게 불린다
	if (!InASC)
	{
		return;
	}

	AbilitySystemComponent = InASC;

	AActor* Actor = InASC->GetOwnerActor();
	if (!Actor)
	{
		return;
	}

	ASPMonsterCharacter* MonsterActor = Cast<ASPMonsterCharacter>(Actor);
	if (!MonsterActor)
	{
		return;
	}

	const FSPDataRow_MonsterInfo* CachedMonsterInfo = MonsterActor->GetCachedMonsterInfo();
	if (CachedMonsterInfo)
	{
		if (NameLabel)
		{
			NameLabel->SetText(FText::FromString(CachedMonsterInfo->Name));
		}
	}

	if (HealthBarWidget)
	{
		MonsterActor->OnHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnHealthChanged);
		MonsterActor->OnMaxHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnMaxHealthChanged);
	}
}
