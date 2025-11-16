// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckDistanceToPos.generated.h"

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API UBTDecorator_CheckDistanceToPos : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	bool CalculateRawConditionValue (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float CheckRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetPositionKey = TEXT("TargetPosition");
};
