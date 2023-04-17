// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API AAI_EnemyController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	

private: 
	/* UPROPERTY(EditAnywhere, Category = "AI_Behavior")
	float DistanceFromPlayer = 200;
	UPROPERTY(EditAnywhere, Category = "AI_Behavior")
	bool shouldFocus = true;
	UPROPERTY(EditAnywhere, Category = "AI_Behavior")
	bool shouldFollow = true;
	*/
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	class UBehaviorTree* AI_EnemyBehavior;
};
