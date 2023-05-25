// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_LastPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_LastPlayerLocation::UBTService_LastPlayerLocation()
{
	NodeName = TEXT("Update Last Known Player Location");
}

void UBTService_LastPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get AI perception component
	UAIPerceptionComponent* PerceptionComponent = OwnerComp.GetAIOwner()->GetPerceptionComponent();
	ensureMsgf(PerceptionComponent != nullptr, TEXT("Perception component is nullptr"));
	if (PerceptionComponent == nullptr)
	{
		return;
	}

	// Get perceived actors from sight stimulus
	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	ensureMsgf(PerceivedActors.Num() > 0, TEXT("No actors perceived"));

	// Find the player character from the perceived actors
	const APlayerCharacter* PlayerCharacter = nullptr;
	for (AActor* PerceivedActor : PerceivedActors)
	{
		PlayerCharacter = Cast<APlayerCharacter>(PerceivedActor);
		if (PlayerCharacter != nullptr)
		{
			break;
		}
	}

	if (PlayerCharacter != nullptr)
	{
		// Player character detected through stimulus
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
	}
	else
	{
		// Player character not detected
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
