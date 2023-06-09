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

	if (!bStartLocationSet)
	{
		//set startlocation and startrotation
		ensureMsgf(OwnerComp.GetBlackboardComponent() != nullptr, TEXT("BB component is nullptr"));
		if (OwnerComp.GetBlackboardComponent() == nullptr) return;

		ensureMsgf(OwnerComp.GetAIOwner() != nullptr, TEXT("AI controller is nullptr"));
		if (OwnerComp.GetAIOwner() == nullptr) return;

		ensureMsgf(OwnerComp.GetAIOwner()->GetPawn() != nullptr, TEXT("AI pawn is nullptr"));
		if (OwnerComp.GetAIOwner()->GetPawn() == nullptr) return;

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),
		                                                     OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsRotator(TEXT("StartRotation"),
		                                                      OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation());
		bStartLocationSet = true;
	}
}
