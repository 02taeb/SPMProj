// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class SPMPROJ_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

// virtual class UWorld* GetWorld() const { return World; };

	//Item vet om sin värld ifall partikeleffekter behöver spawnas vid use
	UPROPERTY(Transient)
	class UWorld* World;

	//Texten när musen är över item i inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseText;

	//bildenm på item i inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;
	
	//vilket Inventory itemet ägs av
	UPROPERTY(BlueprintReadWrite)
	class UInventoryComponent* OwningInventory;

	//Den här variabeln kan ändras om spelarens character klass ändras
	//Vad som ska hända när du använder item
	virtual void Use(class APlayerCharacter* Character);
	
	//Ta bort item ur inventory utan att använda
	void Discard(class APlayerCharacter* Character);

	//Samma som Use() men för Blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse (class APlayerCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
