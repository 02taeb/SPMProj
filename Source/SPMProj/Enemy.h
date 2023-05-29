// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"
//delegate for enemy death
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathSignature);

UCLASS()
class SPMPROJ_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	void EnemyAttackBasic();

	void CalculateHitDirection(const FVector ImpactPoint);

	/*Functions to enable or disable weapon box collison in blueprints*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollison(ECollisionEnabled::Type Collision) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category=Stats)
	class UStatComponent* Stats;

	UPROPERTY(EditDefaultsOnly, Category = Damaged)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* EnemyAttackMontage;
	int32 AttackIndex = 1;

	/*Animation montage för hit react*/
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* EnemyHitReactMontage;
	double HitAngle;

	//Audio
	UPROPERTY()
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* TakeDamageSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* DeathSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* AttackSoundCue;

	void PlaySound(USoundCue* Sound);

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/*Spawn weapon class variable. Used to spawn the weapon on the Enemies right hand socket*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMeleeWeapon> WeaponClass;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* EnemyWeapon;

	void PlayEnemyAttackMontage();

	void PlayEnemyHitReact();
	void SetPlayerLocationAfterOnHit();
	FTimerHandle SetPlayerLocationTimerAfterOnHit;
	UPROPERTY(EditAnywhere, Category= "Enemy react")
	float TimeToReact = 2.0f;
	bool IsAttacking;
	const bool GetIsAttacking();
	//time for attack to finish
	float AttackMontageDuration;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeathBPEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitBPEvent();
	//get stats
	class UStatComponent* GetStats() const;
	//function to call for enemy death delegate //fråga daniel om ni inte fattar
	void Die() const;
	// OnDeath delegate that is triggered when the enemy dies
	FEnemyDeathSignature OnDeath;

	void TargetLockPlayer(std::string);
	void MoveAlongTargetLock();

	void SetTargetIndicator(bool Locked);
	
	FORCEINLINE float GetAttackMontageDuration() const { return AttackMontageDuration; }

private:
	//player TargetLock
	UPROPERTY()
	class APlayerCharacter* PlayerTargetLock = nullptr;
	UPROPERTY(EditAnywhere, Category=" Target Lock")
	float TargetLockDistance = 500.0f;
	UPROPERTY(EditAnywhere, Category= "Target Lock")
	float MoveDistanceFromPlayer = 200.0f;

public:
	void ResetTargetLock();

	//for patrolling enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	TArray<AActor*> PatrolActors;
	UPROPERTY(EditAnywhere, Category="Patrol")
	float PatrolSpeed;
	UPROPERTY(EditAnywhere, Category="Speed")
	float NormalSpeed = 600.0f;
};
