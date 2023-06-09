// Fill out your copyright notice in the Description page of Project Settings.


#include "BossManager.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossManager::ABossManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BossCloud = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Compoenet"));
	if (BossCloud == nullptr) return;
	BossCloud->SetupAttachment(RootComponent);
	ArenaCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	if (ArenaCollider == nullptr) return;
	ArenaCollider->SetupAttachment(RootComponent);
	DummyParentForColls = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	if ( DummyParentForColls== nullptr) return;
	DummyParentForColls->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABossManager::BeginPlay()
{
	Super::BeginPlay();

	// Setup checks
	if (BossCrystals.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss crystals array empty!"));
		bPassedChecks = false;
	}
	if (ArenaCollider == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss Arena collider (Box Component) nullptr!"));
		bPassedChecks = false;
	}
	if (BossCloud == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss Cloud (Static mesh) nullptr"));
		bPassedChecks = false;
	}
	if (!bPassedChecks) return;
	CurrentAttackSpeed = AttackSpeed1;
}

// Called every frame
void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bFightStarted) return;
	if (!bPassedChecks) return;
	
	if (bShouldTimer)
		Timer += DeltaTime;
	else
	{
		for (const AActor* Enemy : SpawnedEnemies)
		{
			if (Enemy == nullptr) continue;
			if (Enemy->IsValidLowLevel() || !Enemy->IsActorBeingDestroyed())
			{
				bAllEnemiesKilled = false;
				break;
			}
		}
		if (bAllEnemiesKilled)
		{
			CleanupAttackStyle2();
			AttackStyle3();
		}
	}

	if (Timer >= CurrentAttackSpeed)
	{
		Timer = 0;
		Attack();
	}
}

void ABossManager::Attack()
{
	switch (CurrentPhase)
	{
	case 1:
		if (AttackCounter++ <= 3)
			AttackStyle1();
		else
			AttackStyle3();
		break;
		
	case 2:
	case 3:
		if (AttackCounter++ <= 3)
			AttackStyle1();
		else
			AttackStyle2();
		break;
		
	default:
		UE_LOG(LogTemp, Warning, TEXT("Switch Boss attack defaulted"));
		break;
	}
}

void ABossManager::AttackStyle1()
{
	
	// Indicate hit
		// Pick random pattern from predetermined
	// Start counting down to hit - dependent on attack speed
	// Make indicator more obvious the closer time gets to attack
	// OnAttack
		// Remove indicator
		// Spawn lightning particles
		// Damage player if standing inside area
		// Spawn impact particles on impact point
}

void ABossManager::AttackStyle2()
{
	bShouldTimer = false;
	const int SpawnAmount = CurrentPhase == 2 ? EnemySpawnAmount1 : EnemySpawnAmount2;
	// Play SpawnEnemy particle system
	SpawnedEnemies.Init(nullptr, SpawnAmount);
	for (int i = 0; i < SpawnAmount; i++)
	{
		// Spawn Enemy
			// AActor* EnemyPtr = SomeNameSpace::SpawnActor(EnemyClass, SomeLoc, FQuat::Identity);
		// Add enemy to SpawnedEnemies
			// SpawnedEnemies.Insert(EnemyPtr, i);
	}
}

void ABossManager::CleanupAttackStyle2()
{
	bAllEnemiesKilled = true;
	// Remove SpawnEnemyParticleSystem
	SpawnedEnemies.Empty();
}

void ABossManager::AttackStyle3()
{
	// Charge crystals attack

	// if (PlayerSucceededAttackStyle)
		CurrentAttackSpeed = CurrentPhase == 1 ? AttackSpeed2 : AttackSpeed3;
	// When attack style is finished:
	bShouldTimer = true;
	AttackCounter = 1;
}

