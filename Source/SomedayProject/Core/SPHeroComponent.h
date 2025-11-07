// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "SPHeroComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPHeroComponent : public UPawnComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	void InitilzePlayerInputComponent();

	void OnInputActionTriggered(const FInputActionValue& Value, FGameplayTag InputTag);
};