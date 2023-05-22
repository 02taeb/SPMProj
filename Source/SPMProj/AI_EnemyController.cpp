// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "Perception/AISenseConfig_Sight.h"

// kolla varför det skapas tre olika AI sight perceptions

AAI_EnemyController::AAI_EnemyController(const FObjectInitializer& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	AIPerceptionComponent->bAutoActivate = true;
	// sight perception
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
}

void AAI_EnemyController::BeginPlay()
{
	Super::BeginPlay();

	ensureMsgf(AI_EnemyBehavior != nullptr, TEXT("Behavoir Tree nullptr"));
	if (AI_EnemyBehavior != nullptr)
	{
		RunBehaviorTree(AI_EnemyBehavior);
	}

	//setup sight values
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	Sight->DetectionByAffiliation.bDetectEnemies = DetectEnemies;
	Sight->DetectionByAffiliation.bDetectNeutrals = DetectNeutrals;
	Sight->DetectionByAffiliation.bDetectFriendlies = DetectFriendlies;

	SetPerceptionComponent(*AIPerceptionComponent);

	// configure sense
	AIPerceptionComponent->ConfigureSense(*Sight);

	// set dominant sense
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
	
}

void AAI_EnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//define which metod to call when perception gets updated 
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_EnemyController::OnPerception);
}

void AAI_EnemyController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//set Blackboard value if stimulus was succesfully sensed
	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), false);
	}
}
