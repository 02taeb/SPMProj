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
	
	ensureMsgf(OwnerComp.GetAIOwner() != nullptr, TEXT("AI controller is Nullptr"));
	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	
	ensureMsgf(OwnerComp.GetAIOwner()->GetPawn() != nullptr, TEXT("AI Pawn is Nullptr"));
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;
	
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	ensureMsgf(Enemy != nullptr, TEXT("Enemy is Nullptr"));
	if(Enemy == nullptr) return EBTNodeResult::Failed;
	
	//Call enemy ResetTarget lock Function
	Enemy->ResetTargetLock();
	
	return EBTNodeResult::Succeeded;	
	
}
