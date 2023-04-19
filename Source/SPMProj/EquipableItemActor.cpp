// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItemActor.h"
#include "PlayerCharacter.h"

AEquipableItemActor::AEquipableItemActor()
{

}


void AEquipableItemActor::Use(class APlayerCharacter* Character)
{
    Super::Use(Character);

    if (Equipped)
    {
        Equipped = false;
    }
    else
    {
        Equipped = true;
    }

}
