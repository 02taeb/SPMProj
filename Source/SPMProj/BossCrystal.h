// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossCrystal.generated.h"

//Deprecated, boss not in final game
UCLASS()
class SPMPROJ_API ABossCrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossCrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Which index in the order this crystal has.
	int IndexOfCrystal = 0;
	// Has this crystal been hit?
	bool bHit = false;
};
