// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveAroundPlayer.h"
#include "AIController.h"
#include "Enemy.h"

UBTTask_MoveAroundPlayer::UBTTask_MoveAroundPlayer()
{
	NodeName = "Target Lock Player and Set MoveAroundPlayerLocation";
}

EBTNodeResult::Type UBTTask_MoveAroundPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	
	if(Enemy == nullptr) return EBTNodeResult::Failed;
	
	Enemy->TargetLockPlayer("no_TP");
	
	return EBTNodeResult::Succeeded;	
	
}
