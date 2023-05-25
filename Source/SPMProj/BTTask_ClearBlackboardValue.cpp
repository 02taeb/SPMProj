// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
    //set name of node
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    //clear selected blackboardkey

    ensureMsgf(OwnerComp.GetBlackboardComponent() != nullptr, TEXT("BB component is nullptr"));
    if (OwnerComp.GetBlackboardComponent() == nullptr) return EBTNodeResult::Failed;
    
    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    return  EBTNodeResult::Succeeded;
}
