// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"

#include "WeaponEquipmentDefinition.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FSPAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * 
 */
class UWeaponEquipmentDefinition : UObject
{
public:
	UWeaponEquipmentDefinition (const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// 스폰할 클래스
	// @TODO 나중에 EquipmentInstance 가 생기면 수정
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
