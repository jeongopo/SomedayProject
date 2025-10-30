// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "WeaponEquipmentDefinition.h"
#include "WeaponManagerComponent.generated.h"

class UWeaponEquipmentDefinition;
/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API UWeaponManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	void EquipWeapon (UWeaponEquipmentDefinition* WeaponDefinition);
	void SpawnWeaponToActor (UWeaponEquipmentDefinition* WeaponDefinition);
	void DestroyEquipmentActors ();

protected:
	UPROPERTY()
	TArray<UWeaponEquipmentDefinition*> WeaponList;

protected:
	UPROPERTY()
	TArray<AActor*> SpawnedActors;

};
