// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionSystem.h"

AAI_EnemyController::AAI_EnemyController()
{
    // AI perception component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    AIPerceptionComponent->bAutoActivate = true;
    SetPerceptionComponent(*AIPerceptionComponent);
    // sight perception
    UAISenseConfig_Sight *SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    // sight variables

    SightConfig->SightRadius = 500.0f;
    SightConfig->LoseSightRadius = 600.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

    AIPerceptionComponent->ConfigureSense(*SightConfig);

    // TSubclassOf<UAISense> InDominantSense
    // AIPerceptionComponent->SetDominantSense();
}
void AAI_EnemyController::BeginPlay()
{
    Super::BeginPlay();
    if (AI_EnemyBehavior != nullptr)
    {
        RunBehaviorTree(AI_EnemyBehavior);
        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn == nullptr)
            return;

        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}
