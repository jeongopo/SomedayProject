// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/SPGameplayEffect_Damage.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AttributeSet.h"

USPGameplayEffect_Damage::USPGameplayEffect_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
}

