// Fill out your copyright notice in the Description page of Project Settings.


#include "BBTask_MoveToArc.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UBBTask_MoveToArc::UBBTask_MoveToArc()
{
	NodeName = "Move to Arc";
}

EBTNodeResult::Type UBBTask_MoveToArc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToArc controller found "));
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (Blackboard)
		{
			FVector TargetLocation = Blackboard->GetValueAsVector("MoveAroundPlayerLocation");
			UE_LOG(LogTemp, Warning, TEXT("MoveToArc controller found %s"), *TargetLocation.ToString());
			FVector OwnerLocation = AIController->GetPawn()->GetActorLocation();

			// Calculate the arc offset
			FVector OffsetDirection = FVector::CrossProduct(TargetLocation - OwnerLocation, FVector::UpVector);
			FVector ArcOffset = OffsetDirection.GetSafeNormal() * MoveAroundPlayerDistance;

			// Calculate the final move-to location with arc offset
			FVector MoveToLocation = TargetLocation + ArcOffset;

			// Rotate the AI towards the target location
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, MoveToLocation);
			AIController->GetPawn()->SetActorRotation(FMath::RInterpTo(AIController->GetPawn()->GetActorRotation(),
																	   TargetRotation, GetWorld()->GetDeltaSeconds(),
																	   RotationInterpSpeed));
			
			
			AIController->MoveToLocation(MoveToLocation);

			UE_LOG(LogTemp, Warning, TEXT("MoveToArc task succeeded."));
			return EBTNodeResult::Succeeded;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("MoveToArc task failed."));
	return EBTNodeResult::Failed;
}
