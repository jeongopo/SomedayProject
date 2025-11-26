// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTNode/BTTask_FindRandomPatrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("OriginPosition"));
    FNavLocation Result;
    if (NavSys->GetRandomPointInNavigableRadius(OriginPos, PatrolRadius, Result))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPosition"), Result.Location);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
