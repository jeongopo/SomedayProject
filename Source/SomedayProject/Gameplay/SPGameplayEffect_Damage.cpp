// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/SPGameplayEffect_Damage.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Core/SPBaseAttributeSet.h"
#include "AttributeSet.h"

USPGameplayEffect_Damage::USPGameplayEffect_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = USPBaseAttributeSet::GetHealthAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(-10.0f);
    Modifiers.Add(ModifierInfo);
}

