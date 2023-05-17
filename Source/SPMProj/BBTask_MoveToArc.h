// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BBTask_MoveToArc.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBBTask_MoveToArc : public UBTTask_MoveTo
{
	GENERATED_BODY()


public:
	UBBTask_MoveToArc();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float RotationInterpSpeed = 100.0f;
	float MoveAroundPlayerDistance = 200.0f;
};
