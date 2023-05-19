// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetTargetLock.h"
#include "AIController.h"
#include "Enemy.h"

UBTTask_ResetTargetLock::UBTTask_ResetTargetLock()
{
	NodeName = "Reset Target Lock";
}

EBTNodeResult::Type UBTTask_ResetTargetLock::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	
	if(Enemy == nullptr) return EBTNodeResult::Failed;
	
	Enemy->ResetTargetLock();
	
	return EBTNodeResult::Succeeded;	
	
}
