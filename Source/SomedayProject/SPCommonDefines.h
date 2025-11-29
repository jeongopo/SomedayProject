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

UENUM(BlueprintType)
enum class ESP_AttackType : uint8
{
	Normal = 0,
	Hard,
	Skill
};

UENUM(BlueprintType)
enum class ESP_AbnormalType :uint8
{
	None        UMETA(DisplayName = "None"),
	Stun        UMETA(DisplayName = "Stun"),
};

