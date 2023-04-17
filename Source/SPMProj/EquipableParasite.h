// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipableParasite.generated.h"

UCLASS()
class SPMPROJ_API AEquipableParasite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableParasite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* StaticMeshComponent;
	class UStatComponent* StatComponentPtr;
	AActor* PlayerActorPtr;

	void OnPickup();
	void OnEquip();
	void OnUnequip();
	void OnPlayerDeath();
};