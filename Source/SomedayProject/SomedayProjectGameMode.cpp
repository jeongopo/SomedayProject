// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomedayProjectGameMode.h"
#include "SomedayProjectCharacter.h"
#include "Core/SPPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASomedayProjectGameMode::ASomedayProjectGameMode()
{
	DefaultPawnClass = ASomedayProjectCharacter::StaticClass();
	PlayerControllerClass = ASPPlayerController::StaticClass();

	//GameStateClass = ALyraGameState::StaticClass();
	//GameSessionClass = ALyraGameSession::StaticClass();
	//ReplaySpectatorPlayerControllerClass = ALyraReplayPlayerController::StaticClass();
	//PlayerStateClass = ALyraPlayerState::StaticClass();
	//HUDClass = ALyraHUD::StaticClass();
}
