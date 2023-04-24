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

	/*Attaches the weapon to the skeleton on */
	void AttachWeaponOnPlayer(USceneComponent* Player, FName SocketLabel);

	
protected:
	virtual void BeginPlay() override;

	/*Callback function for OnComponentBeginOverlap delegate.
	 *Triggered when the BoxCollision component (weapon) overlaps with an object (set to ignore Pawn as not to overlap with the capsule but the objects mesh)*/
	UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() 
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeleeWeaponMesh;
	
	/*A Sphere for detecting overlapping with Character. If overlapping the Character can pick up the weapon. */
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickupZone;

	/*Adding a box component to use for collision overlap events, when attacking. To detect when the weapon hits/overlaps some object.*/
	UPROPERTY(VisibleAnywhere, Category=WeaponProperties)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category=WeaponProperties)
	float DefaultDamage = 25.f;
	UPROPERTY(EditDefaultsOnly, Category=WeaponProperties)
	float HeavyDamage = 50.f;

	/*Box trace starting and ending points*/
	UPROPERTY(VisibleAnywhere) 
	USceneComponent* BTStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BTEnd;

	void CheckBoxHitActorType(AActor* Actor);
public:
	UBoxComponent* GetCollisionBox() const;
};
