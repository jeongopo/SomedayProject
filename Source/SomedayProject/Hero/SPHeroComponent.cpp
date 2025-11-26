// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/SPHeroComponent.h"
#include "SPLogHelper.h"
#include "Hero/SPHeroCharacter.h"
#include "Core/SPDefaultData.h"
#include "EnhancedInputComponent.h"
#include "Core/SPAbilitySystemComponent.h"
#include "Core/SPBaseAttributeSet.h"
#include "SPLogHelper.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"

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

void USPHeroComponent::InitializeWithAbilitySystemComponent(USPAbilitySystemComponent* InASC)
{
	if (InASC == nullptr)
	{
		UE_LOG(LogSPDefault, Error, TEXT("USPHeroComponent::InitializeWithAbilitySystemComponent: InASC is nullptr"));
		return;
	}
	AbilitySystemComponent = InASC;

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USPBaseAttributeSet::GetHealthAttribute()).AddUObject(this, &USPHeroComponent::OnHealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USPBaseAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &USPHeroComponent::OnMaxHealthChanged);
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

void USPHeroComponent::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    OnHealthChangedDelegate.Broadcast(AbilitySystemComponent, ChangeData.OldValue, ChangeData.NewValue);
}

void USPHeroComponent::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    OnMaxHealthChangedDelegate.Broadcast(AbilitySystemComponent, ChangeData.OldValue, ChangeData.NewValue);
}

void USPHeroComponent::BeginPlay()
{
    Super::BeginPlay();

    ASPHeroCharacter* SPCharacter = GetPawn<ASPHeroCharacter>();
    if (SPCharacter == nullptr)
    {
        return;
    }

    InitializePlayerInputComponent();
    InitializeWithAbilitySystemComponent(SPCharacter->GetSPAbilitySystemComponent());
}