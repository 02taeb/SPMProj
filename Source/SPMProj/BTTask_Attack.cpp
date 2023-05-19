// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "Enemy.h"
//#include "Animation/AnimInstance.h"



UBTTask_Attack::UBTTask_Attack()
{
   NodeName = TEXT("Perform MeeleAttack");
	
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;
    AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	
    if(Enemy == nullptr) return EBTNodeResult::Failed;
	
    Enemy->EnemyAttackBasic();
	
    return EBTNodeResult::Succeeded;
}
