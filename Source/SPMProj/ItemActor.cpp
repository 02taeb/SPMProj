// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "InventoryComponent.h"
#include "PlayerCharacter.h"

//Hugo allt

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Lite defaultvärden
    ItemName = FText::FromString("Item");
    UseText = FText::FromString("Use");
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::Use(class APlayerCharacter* Character)
{
    
}
