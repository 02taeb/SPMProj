// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

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

bool UStatComponent::Dead()
{
	return true;
}
