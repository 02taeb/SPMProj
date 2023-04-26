// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_StartLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "AI_EnemyController.h"
#include "GameFramework/Controller.h"

UBTService_StartLocation::UBTService_StartLocation()
{
    NodeName = TEXT("Set Start Location");
}

void UBTService_StartLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    if (i == 0)
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
    i++;
}
