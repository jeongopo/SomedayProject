// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SPCommonDefines.h"
#include "SPGameplayEffect_Damage.generated.h"

UCLASS()
class SOMEDAYPROJECT_API USPGameplayEffect_Damage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	USPGameplayEffect_Damage();

	bool bIsStopAttack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	ESP_AbnormalType AbnormalType = ESP_AbnormalType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	ESP_AttackType AttackType = ESP_AttackType::Normal;
};






