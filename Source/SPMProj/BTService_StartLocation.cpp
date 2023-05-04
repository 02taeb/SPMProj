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
	//set desired behavior for my service.
	bNotifyBecomeRelevant = true;
	bNotifyTick = false;
	bNotifyCeaseRelevant = true;
}

//set startlocation when service becomes relevant 
void UBTService_StartLocation::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	//UE_LOG(LogTemp, Warning, TEXT("startlocation setting"));
	if (!bStartLocationSet)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),
		                                                     OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
		bStartLocationSet = true;
	}
}
