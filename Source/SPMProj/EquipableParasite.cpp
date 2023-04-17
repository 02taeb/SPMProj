// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableParasite.h"

// Sets default values
AEquipableParasite::AEquipableParasite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AEquipableParasite::BeginPlay()
{
	Super::BeginPlay();

	// Set PlayerPtr
	// It would probably be okay to set it already here instead of waiting for pickup
}
	
void AEquipableParasite::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Remove buffs from player
}

// Called every frame
void AEquipableParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipableParasite::OnPickup()
{
	// Hide object in world
	// Set statcomponentptr
	// Add to inventory
	// Allow equipping
}

void AEquipableParasite::OnEquip()
{
	// Attach to socket on player
	// Give buff to player
	// Register as equipped
	// Allow unequipping
	// Remove from inventory? Displayed on UI? Changed appearance in inventory to mark as equipped?
}

void AEquipableParasite::OnUnequip()
{
	// Reverse of OnEquip()
}

void AEquipableParasite::OnPlayerDeath()
{
	// Destroy this
}

