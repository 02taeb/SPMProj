#include "FightingArea.h"
#include "Enemy.h"
#include "StatComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AFightingArea::AFightingArea()
{
	// Set the default values of the door and bounds
	Door = nullptr;
	Bounds = CreateDefaultSubobject<UBoxComponent>("Bounds");
	RootComponent = Bounds;
	
	PrimaryActorTick.bCanEverTick = true;
}

void AFightingArea::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	
		TArray<AActor*> FoundEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
		for (AActor* EnemyActor : FoundEnemies)
		{
			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
			//hittar enemy men overlapping actor not working

			if (Bounds == nullptr || Enemy == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Bounds or Enemy is null"));
				return;
			}
							
			if (!Enemy->IsValidLowLevel() || Enemy->IsPendingKill())
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy is invalid or pending kill"));
				return;
			}
		
			TArray<AActor*> OverlappingActors;
			Bounds->GetOverlappingActors(OverlappingActors);
	
			for (AActor* OverlappingActor : OverlappingActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *OverlappingActor->GetName());
			}
		
			if (Enemy != nullptr && Bounds->IsOverlappingActor(Enemy))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found an enemy in bounds"));
				Enemies.Add(Enemy);
				//rider crash här	
				Enemy->OnDeath.AddDynamic(this, &AFightingArea::CheckEnemiesDead);
			}
		}
	
}

void AFightingArea::BeginPlay()
{
	
	Super::BeginPlay();

//	GetWorld()->OnWorldBeginPlay
	
//	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	// Find all of the enemies in the fighting area
	/*TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
	for (AActor* EnemyActor : FoundEnemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		//hittar enemy men overlapping actor not working

		if (Bounds == nullptr || Enemy == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bounds or Enemy is null"));
			return;
		}
							
		if (!Enemy->IsValidLowLevel() || Enemy->IsPendingKill())
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy is invalid or pending kill"));
			return;
		}
		
		TArray<AActor*> OverlappingActors;
		Bounds->GetOverlappingActors(OverlappingActors);
	
		for (AActor* OverlappingActor : OverlappingActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *OverlappingActor->GetName());
		}
		
		if (Enemy != nullptr && Bounds->IsOverlappingActor(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found an enemy in bounds"));
			Enemies.Add(Enemy);
			Enemy->OnDeath.AddDynamic(this, &AFightingArea::CheckEnemiesDead);
		}
	}*/
}

void AFightingArea::CheckEnemiesDead()
{
	UE_LOG(LogTemp, Warning, TEXT("Checking if enemies are dead"));
	// Check if all enemies are dead
	for (AEnemy* Enemy : Enemies)
	{
		if (!Enemy->GetStats()->Dead())
		{
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("ALl enemies were dead"));

	// Open the door if all enemies are dead
	if (Door != nullptr)
	{
		Door->SetActorEnableCollision(false);
		Door->SetActorHiddenInGame(true);
	}
}
