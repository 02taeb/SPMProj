// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_EnemyController::AAI_EnemyController(const FObjectInitializer &ObjectInitializer)
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    // AIPerceptionComponent->bAutoActivate = true;

    SetPerceptionComponent(*AIPerceptionComponent);

    // sight perception
    Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

    // sight variables
    Sight->SightRadius = 500.0f;
    Sight->LoseSightRadius = 600.0f;
    Sight->PeripheralVisionAngleDegrees = 90.0f;
    Sight->DetectionByAffiliation.bDetectEnemies = true;
    Sight->DetectionByAffiliation.bDetectNeutrals = true;
    Sight->DetectionByAffiliation.bDetectFriendlies = false;

    // configure sense
    AIPerceptionComponent->ConfigureSense(*Sight);
    // set dominant sense
    AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

void AAI_EnemyController::BeginPlay()
{
    Super::BeginPlay();
    if (AI_EnemyBehavior != nullptr)
    {
        RunBehaviorTree(AI_EnemyBehavior);
        if (const APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); PlayerPawn == nullptr)
            return;

        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AAI_EnemyController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Display, TEXT("OnPossess method being called"));
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_EnemyController::OnPerception);
}

void AAI_EnemyController::OnPerception(AActor *Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Display, TEXT("IN onperception method"));
    if (const APlayerCharacter *Player = Cast<APlayerCharacter>(Actor); Player == nullptr)
        return;

    UE_LOG(LogTemp, Display, TEXT("Onperception is calling"));
    // Agent->SetAnimState(Stimulus.WasSuccessfullySensed());

    if (Stimulus.WasSuccessfullySensed())
    {
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), true);
    }
    else
    {
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsFacingTowardsPlayer"), false);
    }
}
