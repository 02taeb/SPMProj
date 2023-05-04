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
}

void AFightingArea::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	// Find all of the enemies in the fighting area
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

		
		if (Enemy != nullptr && Bounds->IsOverlappingActor(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found an enemy in bounds"));
			Enemies.Add(Enemy);
			Enemy->OnDeath.AddDynamic(this, &AFightingArea::CheckEnemiesDead);
		}
	}
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
