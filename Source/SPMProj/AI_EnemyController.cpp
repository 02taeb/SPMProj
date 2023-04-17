// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_EnemyController.h"
#include "Kismet/GameplayStatics.h"

void AAI_EnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AAI_EnemyController::Tick(float DeltaSeconds)
{
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

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
}
