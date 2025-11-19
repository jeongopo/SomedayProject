// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/SPHeroComponent.h"
#include "SPLogHelper.h"
#include "Hero/SPHeroCharacter.h"
#include "Core/SPDefaultData.h"
#include "EnhancedInputComponent.h"
#include "Core/SPAbilitySystemComponent.h"

void USPHeroComponent::InitializePlayerInputComponent()
{
    ASPHeroCharacter* SPCharacter = GetPawn<ASPHeroCharacter>();
    if (SPCharacter == nullptr)
    {
        return;
    }

    const USPDefaultData* DefaultData = SPCharacter->GetDefaultData();
	if (DefaultData == nullptr)
	{
		return;
	}

    if (SPCharacter->InputComponent != nullptr)
    {
        UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(SPCharacter->InputComponent);
        if (EnhancedInputComponent)
        {
            for (const FSPInputActionBindData& BindData : DefaultData->InputActionBindData)
            {
                if (BindData.InputAction.Get())
                {
                     EnhancedInputComponent->BindAction(BindData.InputAction, ETriggerEvent::Triggered, this, &USPHeroComponent::OnInputActionTriggered, BindData.InputTag);

                }
            }
        }
    }
}

void USPHeroComponent::OnInputActionTriggered(const FInputActionValue& Value, FGameplayTag InputTag)
{
    ASomedayProjectCharacter* SPCharacter = GetPawn<ASomedayProjectCharacter>();
    if (SPCharacter == nullptr)
    {
        return;
    }

    if (USPAbilitySystemComponent* ASC = SPCharacter->GetSPAbilitySystemComponent())
    {
        FGameplayTagContainer InputTagContainer;
        InputTagContainer.AddTag(InputTag);

        ASC->TryActivateAbilitiesByTag(InputTagContainer);
    }
}

void USPHeroComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializePlayerInputComponent();
}