// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
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
	AAI_EnemyController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* Agent;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI Perception")
	UAIPerceptionComponent* AIPerceptionComponent;

	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	UBehaviorTree* AI_EnemyBehavior;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	float SightRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	float LoseSightRadius = 600.0f;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	float PeripheralVisionAngleDegrees = 90.0f;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	bool DetectEnemies = true;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	bool DetectNeutrals = true;

	UPROPERTY(EditAnywhere, Category= "AI Perception")
	bool DetectFriendlies = true;

};
