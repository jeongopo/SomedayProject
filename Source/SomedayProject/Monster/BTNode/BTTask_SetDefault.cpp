// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTNode/BTTask_SetDefault.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SomedayProjectCharacter.h"

EBTNodeResult::Type UBTTask_SetDefault::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//나중에 GA로 변경
	if (AActor* Actor = OwnerComp.GetOwner())
	{
		if (ASomedayProjectCharacter* SPChar = Cast<ASomedayProjectCharacter>(Actor))
		{
			SPChar->ResetCharacter();
		}
	}

	OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
	OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetPosition"));
	OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("DistanceToTargetPos"));
	return EBTNodeResult::Type::Succeeded;
}
