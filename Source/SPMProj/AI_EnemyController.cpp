// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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

void AAI_EnemyController::Tick(float DeltaSeconds)
{
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (LineOfSightTo(PlayerPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    } else {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }

    /*
          if (shouldFocus && LineOfSightTo(PlayerPawn, FVector(0)))
          {
              SetFocus(PlayerPawn);
          }
          else
          {
              ClearFocus(EAIFocusPriority::Gameplay);
          }

          if (shouldFollow && LineOfSightTo(PlayerPawn, FVector(0)))
          {
              SetFocus(PlayerPawn);
              MoveToActor(PlayerPawn, DistanceFromPlayer);
          }
          else
          {
              ClearFocus(EAIFocusPriority::Gameplay);
              StopMovement();
          }
          */
}
