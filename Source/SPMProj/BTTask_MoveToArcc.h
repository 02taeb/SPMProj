// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToArcc.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTTask_MoveToArcc : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToArcc();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float RotationInterpSpeed = 100.0f;
	float MoveAroundPlayerDistance = 200000.0f;

	
};



