// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItemActor.h"
#include "EquipableParasite.generated.h"

UENUM()
enum class EAffectedStat {
	None,
	Health,
	Armor,
	AttackDamage
};

UCLASS()
class SPMPROJ_API AEquipableParasite : public AEquipableItemActor
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
	bool bCanEquip = false;
	bool bIsEquipped = false;

	UPROPERTY(EditDefaultsOnly, Category="Parasite")
	bool bUseStaticMesh = false;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	EAffectedStat Stat = EAffectedStat::None;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	float Amount = 0;

	void OnPickup();
	void OnEquip();
	void OnUnequip();
	void OnPlayerDeath();
};
