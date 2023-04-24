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
	AAI_EnemyController();

protected:
	virtual void BeginPlay() override;
	

private: 
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	class UBehaviorTree* AI_EnemyBehavior;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

};
