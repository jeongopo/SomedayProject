#pragma once

#include "GameFramework/PlayerController.h"
#include "SPPlayerController.generated.h"

UCLASS(config = Game)
class ASPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~Begin AController interface
	virtual void OnPossess (APawn* InPawn) override;
	//~End AController interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
};