// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"


UItem::UItem()
{

    //Lite defaultvärden
    Weight = 1.f;
    ItemName = FText::FromString("Item");
    UseText = FText::FromString("Use");

}

void UItem::Use(class APlayerCharacter* Character)
{
    
}
