// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemActor.h"

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

	/*for (AItemActor* Item : DefaultItems)
	{
		AddItem(Item);
	}*/
	// ...
	
}


bool UInventoryComponent::AddItem(AItemActor *Item)
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

bool UInventoryComponent::RemoveItem(AItemActor *Item)
{

	if (Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		int32 index = Items.Find(Item);
		Items.RemoveAt(index);
		
		//Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	
    return false;
}

