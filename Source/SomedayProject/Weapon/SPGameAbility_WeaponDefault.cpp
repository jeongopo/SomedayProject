// Fill out your copyright notice in the Description page of Project Settings.


#include "SomedayProject/Weapon/SPGameAbility_WeaponDefault.h"
#include "SomedayProject/SPLogHelper.h"

void USPGameAbility_WeaponDefault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogSPDefault, Log, TEXT("USPGameAbility_WeaponDefault::ActivateAbility"));
}

void USPGameAbility_WeaponDefault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
