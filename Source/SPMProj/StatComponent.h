// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

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

	float GetInitialMaxHealth() const;
	float GetMaxHealth() const;
	float GetCurrentHealth() const;

	float GetInitialAttackDamage() const;
	float GetCurrentAttackDamage() const;

	float GetInitialArmor() const;
	float GetCurrentArmor() const;

	float GetCurrentStamina() const;
	void DecreaseStamina(float DecreaseAmount);

	void IncreaseMaxHealth(const float Delta);
	void IncreaseAttackDamage(const float Delta);
	void IncreaseArmor(const float Delta);
	
	void TakeDamage(const float Damage);
	void HealHealth(const float HealAmount);

	bool Dead() const;

	std::string GetState() const;
	void SetState(const std::string& SavedState);

	// Current Health, är public så den kan nås i blueprint
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly)
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float NormalAttackCost;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float HeavyAttackCost;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float RollCost;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float JumpCost;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaDelayRate;


	
	void RestoreStamina();

	UPROPERTY(BlueprintReadWrite)
	bool Restore;

private:
	// Health at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialMaxHealth;
	// Current max health with upgrades
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	bool Decreased;

	// Attack damage at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialAttackDamage;
	float CurrentAttackDamage;

	// Damage resistance/Armor at beginning of game
	UPROPERTY(EditDefaultsOnly)
	float InitialArmor;
	float CurrentArmor;
};
