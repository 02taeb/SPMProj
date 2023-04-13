// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

UHealthItem::UHealthItem()
{
   
}

void UHealthItem::Use(class APlayerCharacter *Character)
{
     if (Character)
     {
        Character->Health += HealthToHeal;

        if (OwningInventory)
        {
           // OwningInventory->RemoveItem(this);
        }
        
     }
     
}