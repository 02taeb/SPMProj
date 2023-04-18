// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItemActor.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

AHealthItemActor::AHealthItemActor()
{
   
}

void AHealthItemActor::Use(class APlayerCharacter *Character)
{
    Super::Use(Character);
    
     if (Character)
     {
        Character->Health += HealthToHeal;

        if (OwningInventory)
        {
            OwningInventory->RemoveItem(this);
        }
        
        
     }
     
}

