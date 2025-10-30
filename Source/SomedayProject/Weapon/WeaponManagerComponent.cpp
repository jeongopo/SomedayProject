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

	for (const FSPAbilitySet_GameplayAbility AbilitySet : WeaponDefinition->GrantedGameplayAbilities)
	{
		USPGameplayAbility* AbilityCDO = AbilitySet.Ability->GetDefaultObject<USPGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilitySet.AbilityLevel);
		AbilitySpec.SourceObject = GetWorld(); //쓰는것같지 않으니 아무거나 넘기기
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
		//AbilitySpecHandle 를 관리하기?
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
		//별도의 Instance 관리가 필요할까?
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
}
