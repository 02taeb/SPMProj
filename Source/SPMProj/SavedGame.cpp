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

    // if (USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass())))
    // {
        
    //     //set up optional delegate
    //     FAsyncSaveGameToSlotDelegate SavedDelegate;
        
    //     // USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
    //     //SavedDelegate.BindUObject();

    //     //SavedDelegate.BindUObject

    //     SaveGameInstance->PlayerName = TEXT("PlayerOne");

    //     //Start async save process
    //     //UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance);
    // }
    
}
