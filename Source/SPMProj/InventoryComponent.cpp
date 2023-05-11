// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemActor.h"
#include "EquipableItemActor.h"
#include "EquipableParasite.h"

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
	
}

bool UInventoryComponent::IsFull()
{
	return Items.Num() >= Capacity;
}

bool UInventoryComponent::AddItem(AItemActor *Item)
{
	

	//kanske inte behövs
	if (Item == nullptr)
	{
		return false;
	}
	

	if (IsFull() || !Item)
	{
		return false;
	}

	if (Cast<AEquipableParasite>(Item))
	{
		Cast<AEquipableParasite>(Item)->PlayerActorPtr = this->GetOwner();
	}
	
	
	
	Item->OwningInventory = this;
	
	//En world för items behövs kanske inte nu när de är actors
	//Item vet om sin värld ifall partikeleffekter behöver spawnas vid use
	
	Item->World = GetWorld();
	Items.Add(Item);

	//Update UI
	OnInventoryUpdated.Broadcast();

	 if (AEquipableParasite* Par = Cast<AEquipableParasite>(Item))
	 	Par->OnPickup();

	if (Cast<AEquipableParasite>(Item))
	{
		Cast<AEquipableParasite>(Item)->OnPickup();
	}
	
	
    return true;
}

bool UInventoryComponent::RemoveItem(AItemActor *Item)
{

	if (Item)
	{
		if (Cast<AEquipableItemActor>(Item))
		{
			Cast<AEquipableItemActor>(Item)->Equipped = false;
		}

		if (AEquipableParasite* Par = Cast<AEquipableParasite>(Item))
			Par->OnPlayerDeath();
		
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

