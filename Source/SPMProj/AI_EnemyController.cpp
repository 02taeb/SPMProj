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
    SetFocus(PlayerPawn);
    MoveToActor(PlayerPawn, DistanceFromPlayer);
}
