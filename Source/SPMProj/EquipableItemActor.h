// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "EquipableItemActor.generated.h"

/**
 * 
 */
UCLASS()
class SPMPROJ_API AEquipableItemActor : public AItemActor
{
	GENERATED_BODY()

public:

	AEquipableItemActor();

	UPROPERTY(BlueprintReadWrite)
	bool Equipped;

protected:

	virtual void Use(class APlayerCharacter* Character) override;


};
