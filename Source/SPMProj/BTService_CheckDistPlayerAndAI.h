// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckDistPlayerAndAI.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UBTService_CheckDistPlayerAndAI : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckDistPlayerAndAI();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
