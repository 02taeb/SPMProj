// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthItem.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API UHealthItem : public UItem
{
	GENERATED_BODY()

public:

	UHealthItem();

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float HealthToHeal;

protected:

	virtual void Use(class APlayerCharacter* Character) override;
	
};
