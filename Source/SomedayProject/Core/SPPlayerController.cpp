#include "SPPlayerController.h"

void ASPPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASPPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASPPlayerController::ReceivedPlayer()
{
	//Super::ReceivedPlayer();
}

void ASPPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}
