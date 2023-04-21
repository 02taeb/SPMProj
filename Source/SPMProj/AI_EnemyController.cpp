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
