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

public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool bStartLocationSet = false;
};
