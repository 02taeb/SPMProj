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

	// Find all of the enemies in the fighting area
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
	for (AActor* EnemyActor : FoundEnemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if (Enemy != nullptr && Bounds->IsOverlappingActor(Enemy))
		{
			Enemies.Add(Enemy);
			Enemy->OnDeath.AddDynamic(this, &AFightingArea::CheckEnemiesDead);
		}
	}
}

void AFightingArea::CheckEnemiesDead()
{
	// Check if all enemies are dead
	for (AEnemy* Enemy : Enemies)
	{
		if (!Enemy->GetStats()->Dead())
		{
			return;
		}
	}

	// Open the door if all enemies are dead
	if (Door != nullptr)
	{
		Door->SetActorEnableCollision(false);
		Door->SetActorHiddenInGame(true);
	}
}
