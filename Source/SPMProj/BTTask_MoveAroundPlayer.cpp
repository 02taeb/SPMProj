// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveAroundPlayer.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveAroundPlayer::UBTTask_MoveAroundPlayer()
{
	NodeName = "Target Lock Player and Set MoveAroundPlayerLocation";
}

EBTNodeResult::Type UBTTask_MoveAroundPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ensureMsgf(OwnerComp.GetAIOwner() != nullptr, TEXT("AI controller is Nullptr"));
	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	ensureMsgf(OwnerComp.GetAIOwner()->GetPawn() != nullptr, TEXT("AI pawn is Nullptr"));
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	ensureMsgf(Enemy != nullptr, TEXT("Enemy is Nullptr"));
	if (Enemy == nullptr) return EBTNodeResult::Failed;
	

	ensureMsgf(OwnerComp.GetBlackboardComponent() != nullptr, TEXT("Blackboard is null"));
	bool ShouldTeleport = OwnerComp.GetBlackboardComponent()->GetValueAsBool("ShouldTeleport");

	//call target lock with paramater
	if (ShouldTeleport)
	{
		Enemy->TargetLockPlayer("teleport");
	}
	else
	{
		Enemy->TargetLockPlayer("no_TP");
	}

	return EBTNodeResult::Succeeded;
}
