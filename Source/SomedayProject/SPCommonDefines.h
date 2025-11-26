#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectState : uint8
{
	None        UMETA(DisplayName = "None"),
	Idle        UMETA(DisplayName = "Idle"),
	Moving      UMETA(DisplayName = "Moving"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Dead        UMETA(DisplayName = "Dead")
};