// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "HealthItemActor.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API AHealthItemActor : public AItemActor
{
	GENERATED_BODY()
public:

	AHealthItemActor();

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float HealthToHeal;

protected:

	virtual void Use(class APlayerCharacter* Character) override;
	

};
