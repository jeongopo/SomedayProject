// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/SPGameplayAbility.h"
#include "WeaponEquipmentDefinition.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, Const, BlueprintType)
class UWeaponEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UWeaponEquipmentDefinition (const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// ������ Ŭ����
	// @TODO ���߿� EquipmentInstance �� ����� ����
	/*
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraEquipmentInstance> InstanceType;
	*/

	// From ULyraAbilitySet
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FSPAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// From FLyraEquipmentActorToSpawn
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};
