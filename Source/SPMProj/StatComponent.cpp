// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

#include <string>

#include "NiagaraFunctionLibrary.h"
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
	CurrentStamina = 1;
	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentHealth <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
		// Death anim
		// Respawna bak/Ladda om scen för full restart
			// Konsekvenser av död? Respawna fiender? Förlora stat?
		// Påverka inv ?
		// Save/Load
		// Kallelse till Level blueprint som får sköta det
	}
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

float UStatComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

void UStatComponent::DecreaseStamina(float DecreaseAmount)
{
	//Decreased = true;
	Restore = false;
	CurrentStamina = FMath::Clamp(CurrentStamina - DecreaseAmount, 0.f, 1.f);
}

void UStatComponent::IncreaseMaxHealth(const float Delta)
{
	MaxHealth += Delta;
}

void UStatComponent::IncreaseAttackDamage(const float Delta)
{
	CurrentAttackDamage += Delta;
}

void UStatComponent::IncreaseArmor(const float Delta)
{
	CurrentArmor += Delta;
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

std::string UStatComponent::GetState() const
{
	std::string State = "";
	State += std::to_string(InitialMaxHealth) + ";";
	State += std::to_string(MaxHealth) + ";";
	State += std::to_string(CurrentHealth) + ";";
	State += std::to_string(InitialAttackDamage) + ";";
	State += std::to_string(CurrentAttackDamage) + ";";
	State += std::to_string(InitialArmor) + ";";
	State += std::to_string(CurrentArmor);
	return State;
}

void UStatComponent::SetState(const std::string& SavedState)
{
	int Counter = 0;
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
	InitialArmor = std::stof(Values[5]);
	CurrentArmor = std::stof(Values[6]);
}

void UStatComponent::RestoreStamina(float DeltaTime)
{
	// FTimerHandle StaminaTimer;
	// Decreased = false;
	UE_LOG(LogTemp, Display, TEXT("Restoring"));

	// GetWorld()->GetTimerManager().SetTimer(StaminaTimer, 0.5f, false);

	if (CurrentStamina == 1)
	{
		Restore = false;
		return;
	}
	
	CurrentStamina += DeltaTime / StaminaRestoreRate;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, 1.f);

	return;
	
}

void UStatComponent::SetRestore()
{
	Restore = true;
}
