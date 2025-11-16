// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Core/SPAbilitySystemComponent.h"
#include "SomedayProjectCharacter.generated.h"

struct FInputActionValue;
class USPAbilitySystemComponent;

UCLASS(config=Game)
class ASomedayProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASomedayProjectCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	USPAbilitySystemComponent* GetSPAbilitySystemComponent() { return AbilitySystemComponent; }

protected:
	UPROPERTY()
	TObjectPtr<USPAbilitySystemComponent> AbilitySystemComponent;
};

