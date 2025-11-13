// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponManagerComponent.h"
#include "GameFramework/Character.h"
#include "Core/SPAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UWeaponManagerComponent::EquipWeapon (UWeaponEquipmentDefinition* WeaponDefinition)
{
	if (WeaponDefinition == nullptr)
	{
		return;
	}

	AActor* OwningActor = GetOwner();
	if (OwningActor == nullptr)
	{
		return;
	}

	USPAbilitySystemComponent* ASC = Cast<USPAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
	if (ASC == nullptr)
	{
		return;
	}

	for (const FSPAbilitySet_GameplayAbility& AbilitySet : WeaponDefinition->GrantedGameplayAbilities)
	{
		USPGameplayAbility* AbilityCDO = AbilitySet.Ability->GetDefaultObject<USPGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilitySet.AbilityLevel);
		AbilitySpec.SourceObject = GetWorld();
		AbilitySpec.GetDynamicSpecSourceTags().AppendTags(FGameplayTagContainer(AbilitySet.InputTag));

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
	}
	SpawnWeaponToActor(WeaponDefinition);
}

void UWeaponManagerComponent::SpawnWeaponToActor (UWeaponEquipmentDefinition* WeaponDefinition)
{
	if (APawn* OwningPawn = GetPawn<APawn>())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(WeaponDefinition->ActorToSpawn, FTransform::Identity, OwningPawn);
		NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActor->SetActorRelativeTransform(WeaponDefinition->AttachTransform);
		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, WeaponDefinition->AttachSocket);

		SpawnedActors.Add(NewActor);
	}
}

void UWeaponManagerComponent::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	SpawnedActors.Empty();
}

AActor* UWeaponManagerComponent::GetFirstEquippedWeapon() const
{
	if (SpawnedActors.Num() > 0 && SpawnedActors[0] != nullptr)
	{
		return SpawnedActors[0];
	}
	return nullptr;
}
