// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDistPlayerAndAI.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckDistPlayerAndAI::UBTService_CheckDistPlayerAndAI()
{
	NodeName = "Update IsInRange value depending on distance between AI and Player";
	//notify that tick is allowed (just in case) 
	bNotifyTick = true;
}

void UBTService_CheckDistPlayerAndAI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Call the parent class's TickNode function
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get the player character
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr)
	{
		// Ensure that the player character is valid
		ensureMsgf(false, TEXT("Player character is nullptr"));
		return;
	}

	// Get the blackboard component
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		// Ensure that the blackboard component is valid
		ensureMsgf(false, TEXT("Blackboard component is nullptr"));
		return;
	}

	// Get the location of the enemy
	FVector EnemyLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	// Get the location of the player character
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	// Define the distance threshold
	float DistanceThreshold = 300.0f; // Adjust this value to define the desired distance threshold

	// Check if the player is within the distance threshold
	bool IsInRange = FVector::Dist(EnemyLocation, PlayerLocation) <= DistanceThreshold;

	// Log the value of IsInRange
	UE_LOG(LogTemp, Warning, TEXT("bIsPlayerTooFar: %s"), IsInRange ? TEXT("true") : TEXT("false"));

	if (IsInRange)
	{
		// Set the value of the selected blackboard key to true
		Blackboard->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	else
	{
		// Set the value of the selected blackboard key to false
		Blackboard->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
