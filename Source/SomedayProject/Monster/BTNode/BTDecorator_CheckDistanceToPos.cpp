// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTNode/BTDecorator_CheckDistanceToPos.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_CheckDistanceToPos::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetPositionKey);
			float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetPos);
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToTargetPos"), Distance);

			return Distance <= CheckRadius;
		}
	}
	return false;
}
