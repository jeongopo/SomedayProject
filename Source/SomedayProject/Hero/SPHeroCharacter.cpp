// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPHeroCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Weapon/WeaponManagerComponent.h"
#include "Core/SPAbilitySystemComponent.h"
#include "Core/SPDefaultData.h"
#include "Core/SPBaseAttributeSet.h"
#include "SPLogHelper.h"
#include "Animation/HeroAnimInstance.h"

//////////////////////////////////////////////////////////////////////////
// ASPHeroCharacter

ASPHeroCharacter::ASPHeroCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManagerComponent"));
	HeroComponent = CreateDefaultSubobject<USPHeroComponent>(TEXT("HeroComponent"));
	HeroAttributes = CreateDefaultSubobject<USPBaseAttributeSet>(TEXT("HeroAttributes"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASPHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultWeapon.Get())
	{
		UWeaponEquipmentDefinition* WeaponDefInstance = NewObject<UWeaponEquipmentDefinition>(this, DefaultWeapon);
		if (WeaponDefInstance)
		{
			WeaponManagerComponent->EquipWeapon(WeaponDefInstance);
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USPBaseAttributeSet::GetHealthAttribute()).AddUObject(this, &ASPHeroCharacter::HandleHealthChanged);
	}
	InitializeAttributes();
}

void ASPHeroCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASPHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASPHeroCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPHeroCharacter::Look);
	}
	else
	{
		UE_LOG(LogSPDefault, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASPHeroCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent || !HeroAttributes)
	{
		return;
	}

	const float MaxHealthValue = DefaultData ? DefaultData->MaxHealth : HeroAttributes->GetMaxHealth();
	//const float AttackPowerValue = DefaultData ? DefaultData->AttackPower : HeroAttributes->GetAttackPower();
	//const float DefenseValue = DefaultData ? DefaultData->Defense : HeroAttributes->GetDefense();
	//const float AttackRangeValue = DefaultData ? DefaultData->AttackRange : HeroAttributes->GetAttackRange();

	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetMaxHealthAttribute(), MaxHealthValue);
	AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetHealthAttribute(), MaxHealthValue);
	//AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetAttackPowerAttribute(), AttackPowerValue);
	//AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetDefenseAttribute(), DefenseValue);
	//AbilitySystemComponent->SetNumericAttributeBase(USPBaseAttributeSet::GetAttackRangeAttribute(), AttackRangeValue);
}

void ASPHeroCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	UE_LOG(LogSPDefault, Log, TEXT("Health Changed %f -> %f"), ChangeData.OldValue, ChangeData.NewValue);
}

void ASPHeroCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASPHeroCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASPHeroCharacter::OnDamaged()
{
	UE_LOG(LogSPDefault, Log, TEXT("ASPHeroCharacter::OnDamaged"));

	if (DefaultData && !DefaultData->HitMontage.IsNull())
	{
		UHeroAnimInstance* AnimInstance = Cast<UHeroAnimInstance>(GetMesh()->GetAnimInstance());
		AnimInstance->Montage_Play(DefaultData->HitMontage.Get());
	}
}
