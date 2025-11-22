// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/SPActorWidget_MonsterTop.h"
#include "UI/HUD/SPWidget_HPBar.h"
#include "Monster/SPMonsterCharacter.h"
#include "Core/SPAbilitySystemComponent.h"
#include "Core/SPBaseAttributeSet.h"

void USPActorWidget_MonsterTop::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USPActorWidget_MonsterTop::InitializedWithAbilitySystem(USPAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		return;
	}

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

	if (HealthBarWidget)
	{
		//구조가 조금 이상하긴 하지만.. delegate 관리를 한 곳에서만 하는게 좋지 않을까
		//이 delegate 삭제 관리도 해줘야할까? 없으면 알아서 안보내겠지?
		MonsterActor->OnHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnHealthChanged);
		MonsterActor->OnMaxHealthChangedDelegate.AddDynamic(HealthBarWidget, &USPWidget_HPBar::OnMaxHealthChanged);
	}
}
