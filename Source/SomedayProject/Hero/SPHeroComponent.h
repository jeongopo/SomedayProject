// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameplayEffectTypes.h"
#include "SPHeroComponent.generated.h"

class USPAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSPHealth_AttributeChanged, const USPHeroComponent*, HeroComponent, float, OldValue, float, NewValue);

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPHeroComponent : public UPawnComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	void InitializePlayerInputComponent();
	void OnInputActionTriggered(const FInputActionValue& Value, FGameplayTag InputTag);

	void OnHealthChanged (const FOnAttributeChangeData& ChangeData);
public:
	void InitializeWithAbilitySystemComponent (USPAbilitySystemComponent* InASC);

public:
	FSPHealth_AttributeChanged OnHealthChangedDelegate;
};