// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ResetTargetLock.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTTask_ResetTargetLock : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ResetTargetLock();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
