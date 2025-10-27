// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "SomedayProject/SomedayProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SomedayProject/SPLogHelper.h"

UHeroAnimInstance::UHeroAnimInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UHeroAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	GameplayTagPropertyMap.Initialize(this, ASC);
}

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//AnimInstance 도 Actor에 붙나?
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}

		if (UCharacterMovementComponent* TempComp = Cast<UCharacterMovementComponent>(OwningActor->GetComponentByClass(UCharacterMovementComponent::StaticClass())))
		{
			MoveComp = TempComp;
		}
	}
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MoveComp)
	{
		Velocity = MoveComp->Velocity;
		bHasAcceleratrion = MoveComp->GetCurrentAcceleration().Length() > 0;

		bWalking = Velocity.Length() > 0 && bHasAcceleratrion;

		//UE_LOG(LogSPDefault, Log, TEXT("Falling %d, Velocity 값 %f, Gravity 값 %f, Apex 변화 %f"), bFalling, MoveComp->Velocity.Z, MoveComp->GetGravityZ(),  TimeToJumpApex);
		bJumping = MoveComp->IsFalling() && MoveComp->Velocity.Z > 0;

		if (bJumping)
		{
			TimeToJumpApex = MoveComp->Velocity.Z / (MoveComp->GetGravityZ()) * (-1);
		}
		else
		{
			TimeToJumpApex = 0.f;
		}
	}
}
