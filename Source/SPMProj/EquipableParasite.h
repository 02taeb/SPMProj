// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItemActor.h"
#include "EquipableParasite.generated.h"

class UNiagaraComponent;
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

	UFUNCTION(BlueprintCallable)
	void OnPickup();
	UFUNCTION(BlueprintCallable)
	void OnEquip();
	UFUNCTION(BlueprintCallable)
	void OnUnequip();
	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath();
	UFUNCTION(BlueprintCallable)
	void OnEat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Parasite")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite)
	AActor* PlayerActorPtr;
	virtual void Use(APlayerCharacter* Character) override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsEquipped = false;
	
private:
	UPROPERTY()
	class UStatComponent* StatComponentPtr;
	bool bCanEquip = false;
	float CurrentAmount;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Parasite")
	// UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category="Parasite")
	bool bUseStaticMesh = false;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	EAffectedStat Stat = EAffectedStat::None;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	float StartAmount = 0;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	float OnEatUpgradeAmount = 2;
	UPROPERTY(EditDefaultsOnly, Category="Parasite|Affect")
	float MaxAmount = 10;
	UPROPERTY(EditDefaultsOnly, Category="Parasite")
	class UNiagaraSystem* Particles;
	UPROPERTY()
	UNiagaraComponent* System;

};
