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

	//AnimInstance �� Actor�� �ٳ�?
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

		//Jump
		bJumping = MoveComp->IsFalling() && MoveComp->Velocity.Z > 0;

		if(MoveComp->IsFalling())
		{
			TimeToLand = CalculateTimeToLand();
		}
		else
		{
			TimeToLand = 0.0f;
		}

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

float UHeroAnimInstance::CalculateTimeToLand ()
{
    if(GetOwningActor() == nullptr)
	{
		return 0.0f;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwningActor());
	if(Character == nullptr)
	{
		return 0.0f;
	}

    FVector Location = Character->GetActorLocation();
    const float Gravity = GetWorld()->GetGravityZ();

    float VerticalVelocity = Velocity.Z;
    float CurrentHeight = Location.Z;

    float GroundHeight = 0.0f; // 필요 시 지형을 추적하여 갱신

    float a = 0.5f * Gravity;
    float b = VerticalVelocity;
    float c = CurrentHeight - GroundHeight;


    float Discriminant = b * b - 4 * a * c;
    if (Discriminant < 0)
    {
        return 0.0f;
    }

    // 두 해 중 양수인 시간 선택
    float SqrtDisc = FMath::Sqrt(Discriminant);
    float t1 = (-b + SqrtDisc) / (2 * a);
    float t2 = (-b - SqrtDisc) / (2 * a);

    TimeToLand = (t1 > 0) ? t1 : t2;
    if (TimeToLand < 0)
    {
        TimeToLand = 0.0f;
    }

	UE_LOG(LogSPDefault, Log, TEXT("점프중 Height : %f, GroundHeight : %f, 시간 값 : %f"), CurrentHeight, GroundHeight, TimeToLand);

    return TimeToLand;
}
