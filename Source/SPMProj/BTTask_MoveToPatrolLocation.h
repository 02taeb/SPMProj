// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTTask_MoveToPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToPatrolLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	int32 PatrolIndex = 0;
};
