// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAroundPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTTask_MoveAroundPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveAroundPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
