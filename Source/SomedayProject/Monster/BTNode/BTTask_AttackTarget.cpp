// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BTNode/BTTask_AttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/SPMonsterCharacter.h"
#include "Monster/SPMonsterDataAsset.h"
#include "AbilitySystemComponent.h"

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    ASPMonsterCharacter* Monster = Cast<ASPMonsterCharacter>(AIController->GetPawn());
    if (!Monster)
    {
        return EBTNodeResult::Failed;
    }

    UObject* TargetObj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
    AActor* TargetActor = Cast<AActor>(TargetObj);
    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

	UAbilitySystemComponent* AbilitySystem = Monster->GetAbilitySystemComponent();
	if (!AbilitySystem)
	{
		return EBTNodeResult::Failed;
	}

    USPMonsterDataAsset* MonsterData = Monster->GetMonsterData();
	if (!MonsterData)
	{
		return EBTNodeResult::Failed;
	}

	const float CurrentTime = Monster->GetWorld()->GetTimeSeconds();
	if (CurrentTime - OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("LastAttackTime")) < MonsterData->AttackInterval)
	{
		return EBTNodeResult::Failed;
	}

    float Distance = FVector::Dist(Monster->GetActorLocation(), TargetActor->GetActorLocation());
    if (Distance <= MonsterData->AttackRange)
    {
        if (MonsterData->AttackAbilityClass)
        {
            if (AbilitySystem->TryActivateAbilityByClass(MonsterData->AttackAbilityClass))
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("LastAttackTime"), CurrentTime);
            }
        }
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
