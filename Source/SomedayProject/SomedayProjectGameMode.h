// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SomedayProjectGameMode.generated.h"

class USPDefaultData;

UCLASS(minimalapi)
class ASomedayProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASomedayProjectGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPDefault")
	USPDefaultData* DefaultData;
};



