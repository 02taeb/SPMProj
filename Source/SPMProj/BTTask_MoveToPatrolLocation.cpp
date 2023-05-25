// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToPatrolLocation.h"

#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_MoveToPatrolLocation::UBTTask_MoveToPatrolLocation()
{
	NodeName = "set patrol location";
}

EBTNodeResult::Type UBTTask_MoveToPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	ensureMsgf(Enemy != nullptr, TEXT("Enemy is nullptr, cast failed"));

	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UCharacterMovementComponent* CharacterMovement = Enemy->GetCharacterMovement();

	ensureMsgf(CharacterMovement != nullptr, TEXT("Char_Movement is nullptr"));
	if (CharacterMovement != nullptr)
	{
		CharacterMovement->MaxWalkSpeed = Enemy->PatrolSpeed;
	}
	
	TArray<AActor*> PatrolActors = Enemy->PatrolActors;

	// Check if the patrol index is valid
	if (PatrolIndex < 0 || PatrolIndex >= PatrolActors.Num())
	{
		return EBTNodeResult::Failed;
	}

	// Get the actor at the specified patrol index
	AActor* TargetActor = PatrolActors[PatrolIndex];
	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Set blackboard value with the actor's location
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		Blackboard->SetValueAsVector("PatrolLocation", TargetLocation);
	}


	PatrolIndex++;
	if (PatrolActors.Num() == PatrolIndex)
	{
		PatrolIndex = 0;
	}


	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
