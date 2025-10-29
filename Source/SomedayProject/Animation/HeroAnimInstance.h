// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemComponent.h"
#include "HeroAnimInstance.generated.h"

class UAbilitySystemComponent;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UHeroAnimInstance (const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem (UAbilitySystemComponent* ASC);

protected:
	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation (float DeltaSeconds) override;

	float CalculateTimeToLand();
protected:
	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

private:
	UPROPERTY()
	UCharacterMovementComponent* MoveComp;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "MoveVariable")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "MoveVariable")
	bool bHasAcceleratrion = false;

	//Jump
	UPROPERTY(BlueprintReadOnly, Category = "MoveVariable")
	float TimeToJumpApex = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "MoveVariable")
	float TimeToLand = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "MoveVariable")
	bool bJumping = false;
	

	UPROPERTY(BlueprintReadOnly, Category = "MoveVariable")
	bool bWalking = false;
};
