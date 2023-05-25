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

	if (AIPerceptionComponent == nullptr) return;
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

	ensureMsgf(Sight != nullptr, TEXT("Sight configt is nullptr"));
	if (Sight == nullptr) return;
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	Sight->DetectionByAffiliation.bDetectEnemies = DetectEnemies;
	Sight->DetectionByAffiliation.bDetectNeutrals = DetectNeutrals;
	Sight->DetectionByAffiliation.bDetectFriendlies = DetectFriendlies;


	SetPerceptionComponent(*AIPerceptionComponent);

	// configure sense
	if (AIPerceptionComponent == nullptr) return;
	AIPerceptionComponent->ConfigureSense(*Sight);

	// set dominant sense
	if (Sight->GetSenseImplementation() == nullptr) return;
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
	// Set Blackboard value if stimulus was successfully sensed of a player
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);

	if (Player != nullptr)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), true);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), false);
		}
	}
}
