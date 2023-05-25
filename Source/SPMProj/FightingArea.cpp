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
	
	//timer to allow enemies to be set up before the fighting area. This way, the fighting area can find enemies in its defined bounds and not nullptrs."
	GetWorldTimerManager().SetTimer(SetTimer, this, &AFightingArea::SetUpFightingArea, 0.5f, false);
}

void AFightingArea::SetUpFightingArea()
{
	UE_LOG(LogTemp, Warning, TEXT("Set up arena "));

	// Find all of the enemies in the fighting area
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
	for (AActor* EnemyActor : FoundEnemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);

		// Ensure that the bounds and enemy are not null
		ensureMsgf(Bounds != nullptr || Enemy != nullptr, TEXT("Bounds or Enemy is null"));
		if (Bounds == nullptr || Enemy == nullptr) return;

		// If the enemy is within the bounds, add it to the list of enemies
		if (Bounds->IsOverlappingActor(Enemy))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found an enemy in bounds"));
			Enemies.Add(Enemy);
			// Bind to the OnDeath event of the enemy to check if all enemies are dead
			// TODO: Might be causing crashes, debug with rider and see stack trace
			Enemy->OnDeath.AddDynamic(this, &AFightingArea::CheckEnemiesDead);
		}
	}
	EnemiesLeft(Enemies.Num());
}

void AFightingArea::CheckEnemiesDead()
{
	int counter = 0;
	bool bAnyEnemyRemains = false;
	// Check if all enemies are dead
	for (AEnemy* Enemy : Enemies)
	{
		if (!Enemy->GetStats()->Dead())
		{
			bAnyEnemyRemains = true;
			counter++;
		}
	}
	if (bAnyEnemyRemains)
	{
		EnemiesLeft(counter);
		return;
	}
	// Open the door if all enemies are dead
	//Ensure door is not nullptr
	ensureMsgf(Door != nullptr, TEXT("Door is nullptr"));
	if (Door != nullptr)
	{
		//Open door
		OpenDoor();
		//Door->SetActorEnableCollision(false);
		//Door->SetActorHiddenInGame(true);
	}
}

