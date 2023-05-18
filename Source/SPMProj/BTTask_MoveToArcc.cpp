// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToArcc.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToArcc.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_MoveToArcc::UBTTask_MoveToArcc()
{
	NodeName = "Move to Arc";
}

EBTNodeResult::Type UBTTask_MoveToArcc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		
			//FNavPathSharedPtr NavPath = ; 
			
			AIController->MoveTo(MoveToLocation);
			//EPathFollowingRequestResult::Type Result = AIController->MoveTo(MoveToLocation, &NavPath);
			
			//UE_LOG(LogTemp, Warning, TEXT("MoveToLocation result: %d"), static_cast<int32>(Result));
			
			UE_LOG(LogTemp, Warning, TEXT("MoveToArc task succeeded."));
			return EBTNodeResult::Succeeded;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("MoveToArc task failed."));
	return EBTNodeResult::Failed;
}

