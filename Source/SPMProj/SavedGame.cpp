// Fill out your copyright notice in the Description page of Project Settings.


#include "SavedGame.h"
#include "Kismet/GameplayStatics.h"
#include "ItemActor.h"
#include "PlayerCharacter.h"

USavedGame::USavedGame()
{

    PlayerPosition = FVector (0.f, 0.f, 0.f);
    SaveSlotName = TEXT("TestSaveSlot");
    UserIndex = 0;
}
