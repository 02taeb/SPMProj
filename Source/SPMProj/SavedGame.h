// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedGame.generated.h"

//Hugo allt

/**
 * 
 */
UCLASS()
class SPMPROJ_API USavedGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	USavedGame();

	//kanske inte används om checkpoints används istället
	UPROPERTY(EditAnywhere, Category = "Generic")
	FVector PlayerPosition;

	//Health att spara ner kanske inte behövs om vi kör checkpoints som bonfires
	/*UPROPERTY(VisibleAnywhere, Category = "Generic")
	std::string StatsState = "";*/

	//Används inte men kanske något sånt i framtiden
	UPROPERTY(VisibleAnywhere, Category = "Generic")
	FVector CheckpointLocation;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	FRotator CheckpointRotation;

	UPROPERTY()
	TArray <float> PlayerState;

	UPROPERTY()
	TArray <AActor*> Chests;

	UPROPERTY()
	bool hasWeapon;

	//Hela inventory som sparas
	//UPROPERTY(VisibleAnywhere, Category = "Generic")
	//TArray <class AItemActor*> CurrentItems;

	//Alla items som är equipped när spelaren sparar, behövs nog inte längre
	//UPROPERTY(VisibleAnywhere, Category = "Generic")
	//TArray <class AEquipableItemActor*> EquippedItems;

	//namnet på save slot, ändras inte nu så finns bara en save
	UPROPERTY(VisibleAnywhere, Category = "Generic")
    FString SaveSlotName;

	//Vet inte exakt vad userindex är men behövs för att spara, just nu är den alltid 0
    UPROPERTY(VisibleAnywhere, Category = "Generic")
    uint32 UserIndex;
};
