// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Core/SPAbilitySystemComponent.h"
#include "SPCommonDefines.h"
#include "Components/WidgetComponent.h"
#include "SomedayProjectCharacter.generated.h"

struct FInputActionValue;
class USPAbilitySystemComponent;
class UWidgetComponent;

UCLASS(config=Game)
class ASomedayProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASomedayProjectCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TObjectPtr<USPAbilitySystemComponent> GetSPAbilitySystemComponent() { return AbilitySystemComponent; }
	virtual void ResetCharacter () {};
	virtual void InitializeAttributes() {};

	//widget component
	virtual void InitializeWidget() {};
	virtual void UpdateWidget() {};

	template <class T>
	inline T* GetAttachedWidget ()
	{
		if (WidgetComponent && WidgetComponent->GetUserWidgetObject())
		{
			return Cast<T>(WidgetComponent->GetUserWidgetObject());
		}
		return nullptr;
	}

	EObjectState GetObjectState() { return CharacterState;	}

protected:
	UPROPERTY()
	TObjectPtr<USPAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;

	EObjectState CharacterState;
};
