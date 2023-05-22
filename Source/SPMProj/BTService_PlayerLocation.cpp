// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update Player Location");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//Set player location if player is not null 
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ensureMsgf(PlayerPawn != nullptr, TEXT("Player is Nullptr"));
	if (PlayerPawn != nullptr) OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	
}
