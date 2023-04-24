// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SPMPROJ_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void EnemyAttackBasic();

	/*Functions to enable or disable weapon box collison in blueprints*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollison(ECollisionEnabled::Type Collision);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category=Stats)
	class UStatComponent* Stats;

	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* EnemyAttackMontage;

	/*Spawn weapon class variable. Used to spawn the weapon on the Enemies right hand socket*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMeleeWeapon> WeaponClass;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* EquipedWeapon;
	
	void PlayEnemyAttackMontage();
};
