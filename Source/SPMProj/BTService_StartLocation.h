// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_StartLocation.generated.h"

/**
 *
 */
UCLASS()
class SPMPROJ_API UBTService_StartLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_StartLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	private:
	int i = 0;
};
