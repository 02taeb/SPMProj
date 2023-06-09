// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

#include <string>

#include "NiagaraFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	/*I guess...*/
	CurrentHealth = InitialMaxHealth; //Ska ändras vara dynamisk
	CurrentStamina = MaxStamina;
	InitialMaxStamina = MaxStamina;
	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (CurrentHealth <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
		// Death anim
		// Respawna bak/Ladda om scen för full restart
			// Konsekvenser av död? Respawna fiender? Förlora stat?
		// Påverka inv ?
		// Save/Load
		// Kallelse till Level blueprint som får sköta det
	} */
}

float UStatComponent::GetInitialMaxHealth() const
{
	return InitialMaxHealth;
}

float UStatComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UStatComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UStatComponent::GetInitialAttackDamage() const
{
	return InitialAttackDamage;
}

float UStatComponent::GetCurrentAttackDamage() const
{
	return CurrentAttackDamage;
}

float UStatComponent::GetInitialArmor() const
{
	return InitialArmor;
}

float UStatComponent::GetCurrentArmor() const
{
	return CurrentArmor;
}

//Hugo
float UStatComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

//Hugo
float UStatComponent::GetMaxStamina() const
{
	return MaxStamina;
}

//Hugo
void UStatComponent::DecreaseStamina(float DecreaseAmount)
{
	//Decreased = true;
	Restore = false;
	CurrentStamina = FMath::Clamp(CurrentStamina - DecreaseAmount, 0.f, MaxStamina);
}

void UStatComponent::IncreaseMaxHealth(const float Delta)
{
	if (MaxHealth + Delta < InitialMaxHealth)
		return;
	MaxHealth += Delta;
	OnMaxHealthUpdated.Broadcast(Delta);
}

void UStatComponent::IncreaseAttackDamage(const float Delta)
{
	if (CurrentAttackDamage + Delta < InitialAttackDamage)
		return;
	CurrentAttackDamage += Delta;
}

void UStatComponent::IncreaseArmor(const float Delta)
{
	if (CurrentArmor + Delta < InitialArmor)
		return;
	CurrentArmor += Delta;
}
//Hugo
void UStatComponent::IncreaseMaxStamina(const float Delta)
{
	if (MaxStamina + Delta < InitialMaxStamina)
		return;

	MaxStamina += Delta;
	OnMaxStaminaUpdated.Broadcast(Delta);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, this, &UStatComponent::SetRestore, StaminaDelayRate, false);
}

void UStatComponent::TakeDamage(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Player received %f damage and lost %f health. New health is %f"),
			Damage, CurrentHealth - Damage, CurrentHealth);
	//Death can be called here if 0...
	/*CurrentHealth -= Damage * (1 / CurrentArmor);   Commented out for testing
	UE_LOG(LogTemp, Warning, TEXT("Player received %f damage and lost %f health. New health is %f"),
		Damage, Damage * (1 / CurrentArmor), CurrentHealth);*/
}

void UStatComponent::HealHealth(const float HealAmount)
{
	CurrentHealth += FMath::Min(MaxHealth - CurrentHealth, HealAmount);
}

bool UStatComponent::Dead() const
{
	return CurrentHealth <= 0.f;
}

TArray <float> UStatComponent::GetState() const
{
	//kanske göra om state till en array istället
	/*
	std::string State = "";
	State += std::to_string(InitialMaxHealth) + ";";
	State += std::to_string(MaxHealth) + ";";
	State += std::to_string(CurrentHealth) + ";";
	State += std::to_string(InitialAttackDamage) + ";";
	State += std::to_string(CurrentAttackDamage) + ";";
	State += std::to_string(MaxStamina) + ";";
	State += std::to_string(CurrentStamina);
	*/

	TArray <float> State{InitialMaxHealth, MaxHealth, CurrentHealth, InitialAttackDamage, CurrentAttackDamage, MaxStamina, StaminaUpgrades, HealthUpgrades, AttackUpgrades};
	return State;
}

void UStatComponent::SetState(TArray <float> SavedState)
{
	/*int Counter = 0;
	std::string Values[7];
	std::string Temp = "";
	
	for (int i = 0; i < SavedState.length(); i++)
	{
		if (SavedState[i] == ';')
		{
			Values[Counter++] = Temp;
			Temp = "";
		}
		Temp += SavedState[i];
	}

	InitialMaxHealth = std::stof(Values[0]);
	MaxHealth = std::stof(Values[1]);
	CurrentHealth = std::stof(Values[2]);
	InitialAttackDamage = std::stof(Values[3]);
	CurrentAttackDamage = std::stof(Values[4]);
	MaxStamina = std::stof(Values[5]);
	IncreaseMaxStamina(std::stof(Values[5]) - 100);
	CurrentStamina = MaxStamina;*/

	InitialMaxHealth = SavedState[0];
	IncreaseMaxHealth(SavedState[1]-100);
	CurrentHealth = SavedState[2];
	InitialAttackDamage = SavedState[3];
	CurrentAttackDamage = SavedState[4];
	IncreaseMaxStamina(SavedState[5]-100);
	CurrentStamina = MaxStamina;

	StaminaUpgrades = SavedState[6];
	HealthUpgrades = SavedState[7];
	AttackUpgrades = SavedState[8];

}

//Hugo
void UStatComponent::RestoreStamina(float DeltaTime)
{
	// FTimerHandle StaminaTimer;
	// Decreased = false;

	// GetWorld()->GetTimerManager().SetTimer(StaminaTimer, 0.5f, false);

	if (CurrentStamina == MaxStamina)
	{
		Restore = false;
		return;
	}
	
	CurrentStamina += DeltaTime / StaminaRestoreRate;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);

	return;
	
}

//Hugo
void UStatComponent::SetRestore()
{
	Restore = true;
}
