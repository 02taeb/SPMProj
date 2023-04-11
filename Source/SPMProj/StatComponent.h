// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPMPROJ_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetIntitialMaxHealth();
	float GetMaxHealth();
	float GetCurrentHealth();

	float GetInitialAttackDamage();
	float GetCurrentAttackDamage();

	float GetInitialArmor();
	float GetCurrentArmor();
private:
	// Health at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialMaxHealth;
	// Current max health with upgrades
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	// Current health
	float CurrentHealth;

	// Attack damage at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialAttackDamage;
	float CurrentAttackDamage;

	// Damage resistance/Armor at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialArmor;
	float CurrentArmor;
};
