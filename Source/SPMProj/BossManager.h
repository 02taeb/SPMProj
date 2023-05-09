// https://docs.google.com/document/d/1zbFGIPhauUL5gDWVG89M4AJs5NrDjQq4cCItJ8cFN8I/edit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossManager.generated.h"

class AEnemy;
class UBoxComponent;
class UNiagaraSystem;
class ABossCrystal;

UCLASS()
class SPMPROJ_API ABossManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Index of current crystal that needs to be hit.
	int CurrentCrystal = 0;
	int CurrentPhase = 1;
	float CurrentAttackSpeed;
	int Timer = 0;
	int AttackCounter = 1;
	bool bShouldTimer = true;
	bool bFightStarted = false;
	bool bPassedChecks = true;
	bool bAllEnemiesKilled = true;
	TArray<AActor*> SpawnedEnemies;
	USceneComponent* DummyParentForColls;

	UPROPERTY(EditAnywhere, Category = "BossFight|Values")
	float AttackSpeed1 = 10;
	UPROPERTY(EditAnywhere, Category = "BossFight|Values")
	float AttackSpeed2 = 20;
	UPROPERTY(EditAnywhere, Category = "BossFight|Values")
	float AttackSpeed3 = 30;
	UPROPERTY(EditAnywhere, Category = "BossFight|Values")
	int EnemySpawnAmount1 = 1;
	UPROPERTY(EditAnywhere, Category = "BossFight|Values")
	int EnemySpawnAmount2 = 2;

	UPROPERTY(EditAnywhere, Category = "BossFight|Particles")
	UNiagaraSystem* Lightning;
	UPROPERTY(EditAnywhere, Category = "BossFight|Particles")
	UNiagaraSystem* Sparks;
	UPROPERTY(EditAnywhere, Category = "BossFight|Particles")
	UNiagaraSystem* Impact;
	UPROPERTY(EditAnywhere, Category = "BossFight|Particles")
	UNiagaraSystem* SpawnEnemies;
	
	UPROPERTY(EditAnywhere, Category = "BossFight|Crystals")
	TArray<TSoftObjectPtr<ABossCrystal>> BossCrystals;

	
	UStaticMeshComponent* BossCloud;
	UBoxComponent* ArenaCollider;
	
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern1;
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern2;
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern3;
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern4;
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern5;
	UPROPERTY(EditAnywhere, Category = "BossFight|AttackPatterns")
	TArray<FComponentReference> AttackPattern6;

	UPROPERTY(EditAnywhere, Category = "BossFight|MinionClass")
	TSubclassOf<AEnemy> EnemyClass;
	
	void Attack();
	void AttackStyle1();
	void AttackStyle2();
	void CleanupAttackStyle2();
	void AttackStyle3();
};
