// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeleeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee Weapon Mesh"));
	RootComponent = MeleeMesh;
	
	PickupZone = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-up zone"));
	PickupZone->SetupAttachment(GetRootComponent());

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());

	BTStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Point"));
	BTStart->SetupAttachment(GetRootComponent());

	BTEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Point"));
	BTEnd->SetupAttachment(GetRootComponent());
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

