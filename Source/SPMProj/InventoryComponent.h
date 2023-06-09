// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

//Hugo allt

//BLueprint binds to this to update UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPMPROJ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(class AItemActor* Item);
	UFUNCTION(BLueprintCallable)
	bool RemoveItem(class AItemActor* Item);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<class AItemActor*> Items;

    UFUNCTION(BlueprintCallable)
    bool IsFull();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



		
};
