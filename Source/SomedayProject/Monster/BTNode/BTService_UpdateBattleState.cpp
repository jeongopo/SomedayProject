// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTNode/BTService_UpdateBattleState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_UpdateBattleState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			FVector OriginPos = BlackboardComp->GetValueAsVector(TEXT("OriginPosition"));
			float Distance = FVector::Dist(AIPawn->GetActorLocation(), OriginPos);
			BlackboardComp->SetValueAsFloat(TEXT("DistanceToOriginPos"), Distance);
		}
	}
}
