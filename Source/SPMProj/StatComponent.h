// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


//Hugo (det som används av Stamina)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthUpdated, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaUpdated, float, Delta);


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

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	
	float GetCurrentHealth() const;

	float GetInitialAttackDamage() const;
	float GetCurrentAttackDamage() const;

	float GetInitialArmor() const;
	float GetCurrentArmor() const;

	float GetCurrentStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	void DecreaseStamina(float DecreaseAmount);

	void IncreaseMaxHealth(const float Delta);
	void IncreaseAttackDamage(const float Delta);
	void IncreaseArmor(const float Delta);
	void IncreaseMaxStamina(const float Delta);

	//test för blueprint
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnMaxHealthUpdated OnMaxHealthUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnMaxStaminaUpdated OnMaxStaminaUpdated;
	
	void TakeDamage(const float Damage);
	void HealHealth(const float HealAmount);

	bool Dead() const;

	TArray <float> GetState() const;
	void SetState(TArray <float> State);

	// Current Health, är public så den kan nås i blueprint
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly)
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly)
	float MaxStamina;

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
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRestoreRate;


	UFUNCTION(BlueprintCallable)
	void RestoreStamina(float DeltaTime);

	void SetRestore();

	UPROPERTY(BlueprintReadWrite)
	bool Restore;

	UPROPERTY(BlueprintReadWrite)
	float StaminaUpgrades;
	UPROPERTY(BlueprintReadWrite)
	float AttackUpgrades;
	UPROPERTY(BlueprintReadWrite)
	float HealthUpgrades;

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
	
	float InitialMaxStamina;
	FTimerHandle StaminaTimer;
};
