// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class SPMPROJ_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeleeWeapon();
	
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	/*Callback function for OnComponentBeginOverlap delegate.
	 *Triggered when the BoxCollision component (weapon) overlaps with an object (set to ignore Pawn as not to overlap with the capsule but the objects mesh)*/
	UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeleeMesh;
	
	/*A Sphere for detecting overlapping with Character. If overlapping the Character can pick up the weapon. */
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickupZone;

	/*Adding a box component to use for collision overlap events, when attacking. To detect when the weapon hits/overlaps some object.*/
	UPROPERTY(VisibleAnywhere, Category=WeaponProperties)
	class UBoxComponent* WeaponBox;

	/*Box trace starting and ending points*/
	UPROPERTY(VisibleAnywhere) 
	USceneComponent* BTStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BTEnd;
};
