// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_AbortIfNotAtLocation.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


UBTDecorator_AbortIfNotAtLocation::UBTDecorator_AbortIfNotAtLocation()
{
	NodeName = "Abort If Not At Location ";
	bNotifyTick = true;
	
}
 // fungrar ej 
void UBTDecorator_AbortIfNotAtLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Node is ticking"));
	//CalculateRawConditionValue(OwnerComp, NodeMemory);
}


bool UBTDecorator_AbortIfNotAtLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr)
	{
		return false;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		return false;
	}

	FVector EnemyLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float DistanceThreshold = 90.0f; // Adjust this value to define the desired distance threshold

	bool bIsPlayerTooFar = FVector::Dist(EnemyLocation, PlayerLocation) > DistanceThreshold;

	UE_LOG(LogTemp, Warning, TEXT("bIsPlayerTooFar: %s"), bIsPlayerTooFar ? TEXT("true") : TEXT("false"));
	if (bIsPlayerTooFar)
	{
		OwnerComp.RequestExecution(EBTNodeResult::Aborted);
	}

	return bIsPlayerTooFar;
}
