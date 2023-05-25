// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_AbortIfNotAtLocation.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTDecorator_AbortIfNotAtLocation : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_AbortIfNotAtLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};


