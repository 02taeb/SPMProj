// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ensureMsgf(PlayerPawn != nullptr, TEXT("Player is nullptr"));
	if (PlayerPawn == nullptr)
		return;
	
	ensureMsgf(OwnerComp.GetAIOwner() != nullptr, TEXT("AI controlller is nullptr"));
	if (OwnerComp.GetAIOwner() == nullptr)
		return;

	//if fetched boolean value is true, set player location, else clear value
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsFacingTowardsPlayer"))
	{
		OwnerComp.GetBlackboardComponent()->
		          SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
