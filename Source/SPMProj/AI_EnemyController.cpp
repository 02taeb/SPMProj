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

	if (AI_EnemyBehavior != nullptr)
	{
		RunBehaviorTree(AI_EnemyBehavior);
	}
	
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	Sight->DetectionByAffiliation.bDetectEnemies = DetectEnemies;
	Sight->DetectionByAffiliation.bDetectNeutrals = DetectNeutrals;
	Sight->DetectionByAffiliation.bDetectFriendlies = DetectFriendies;
	
	SetPerceptionComponent(*AIPerceptionComponent);

	// configure sense
	AIPerceptionComponent->ConfigureSense(*Sight);
	
	// set dominant sense
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	UE_LOG(LogTemp, Warning, TEXT("sightRadius = %f"), SightRadius);
	UE_LOG(LogTemp, Warning, TEXT("LoseSightRadius = %f"), LoseSightRadius);
	UE_LOG(LogTemp, Warning, TEXT("angle = %f"), PeripheralVisionAngleDegrees);
	
}

FPathFollowingRequestResult AAI_EnemyController::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
	FAIMoveRequest Dest;
	FAIMoveRequest MoveReq(Dest);
	bool bUsePathfinding = true;
	MoveReq.SetUsePathfinding(bUsePathfinding);
	bool bAllowPartialPaths = true;
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	bool bProjectDestinationToNavigation = true;
	MoveReq.SetProjectGoalLocation(bProjectDestinationToNavigation);
	TSubclassOf<UNavigationQueryFilter> DefaultNavigationFilterClasss = nullptr ;
	TSubclassOf<UNavigationQueryFilter>::TClassType* FilterClass = nullptr;
	MoveReq.SetNavigationFilter(DefaultNavigationFilterClasss);
	float AcceptanceRadius = 10.0f;
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	bool bStopOnOverlap = false;
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	bool bCanStrafe = true;
	MoveReq.SetCanStrafe(bCanStrafe);
	
	return Super::MoveTo(MoveRequest, OutPath);
}


void AAI_EnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possessed AI Controller: %s"), *GetName());
	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *InPawn->GetName());
	}


	//UE_LOG(LogTemp, Display, TEXT("OnPossess method being called"));
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_EnemyController::OnPerception);
}

void AAI_EnemyController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Display, TEXT("IN onperception method"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
	if ( Player == nullptr)
		return;

	//UE_LOG(LogTemp, Display, TEXT("Onperception is calling"));
	// Agent->SetAnimState(Stimulus.WasSuccessfullySensed());

	// SetFocus(Stimulus.WasSuccessfullySensed() ? Player : nullptr);
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AAI_EnemyController::setBoolBlackBoardValue, 2.f, false);
	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), false);
	}
}

