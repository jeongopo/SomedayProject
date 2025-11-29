// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Hero/SPHeroComponent.h"
#include "Core/SPActor.h"
#include "SomedayProjectCharacter.h"
#include "SPHeroCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USPAbilitySystemComponent;
class UWeaponManagerComponent;
class UWeaponEquipmentDefinition;
class USPHeroComponent;
class USPDefaultData;
class USPBaseAttributeSet;

UCLASS()
class ASPHeroCharacter : public ASomedayProjectCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;


public:
	ASPHeroCharacter();

	USPDefaultData* GetDefaultData() const { return DefaultData ? DefaultData : nullptr; }

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void OnDamaged (ESP_AttackType InAttackType, ESP_AbnormalType InAbnormalType) override;

protected:
	void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitializeAttributes() override;

	void HandleHealthChanged (const FOnAttributeChangeData& ChangeData);

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWeaponEquipmentDefinition> DefaultWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero", meta = (AllowPrivateAccess = "true"))
	USPDefaultData* DefaultData;

	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadOnly, Category = "Hero")
	TObjectPtr<USPBaseAttributeSet> HeroAttributes;

protected:
	UPROPERTY()
	TObjectPtr<UWeaponManagerComponent> WeaponManagerComponent;

	UPROPERTY()
	TObjectPtr<USPHeroComponent> HeroComponent;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

