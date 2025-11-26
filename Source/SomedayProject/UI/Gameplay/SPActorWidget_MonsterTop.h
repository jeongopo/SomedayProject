// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPActorWidget_MonsterTop.generated.h"

class USPWidget_HPBar;
class UTextBlock;
class USPAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPActorWidget_MonsterTop : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeOnInitialized() override;

public:
	void InitializedWithAbilitySystem (USPAbilitySystemComponent* InASC);
	TObjectPtr<USPAbilitySystemComponent> AbilitySystemComponent;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	USPWidget_HPBar* HealthBarWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NameLabel;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* LevelLabel;
};
