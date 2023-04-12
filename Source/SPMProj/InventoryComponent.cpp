// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	//Capacity för inventory, kan ändras i editorn
	Capacity = 20;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//Adderar items som finns i inventory till att börja med

	for (UItem* Item : DefaultItems)
	{
		AddItem(Item);
	}
	// ...
	
}


bool UInventoryComponent::AddItem(UItem *Item)
{

	//kanske inte behövs
	if (Item == nullptr)
	{
		return false;
	}
	

	if (Items.Num() >= Capacity || !Item)
	{
		return false;
	}
	

	Item->OwningInventory = this;

	//Item vet om sin värld ifall partikeleffekter behöver spawnas vid use
	
	Item->World = GetWorld();
	Items.Add(Item);

	//Update UI
	OnInventoryUpdated.Broadcast();

    return true;
}

bool UInventoryComponent::RemoveItem(UItem *Item)
{

	if (Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	
    return false;
}

