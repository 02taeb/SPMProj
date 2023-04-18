// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedGame.generated.h"



/**
 * 
 */
UCLASS()
class SPMPROJ_API USavedGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	USavedGame();

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	FString PlayerName;

	UPROPERTY(EditAnywhere)
	FVector PlayerPosition;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	UObject* Checkpoint;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	TArray <class AItemActor*> CurrentItems;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
	TArray <class AEquipableItemActor*> EquippedItems;

	UPROPERTY(VisibleAnywhere, Category = "Generic")
    FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = "Generic")
    uint32 UserIndex;
};
