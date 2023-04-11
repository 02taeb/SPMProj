// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraTransparencyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPMPROJ_API UCameraTransparencyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraTransparencyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetTransparencyDegree();
	// Will return nullptr on no blocking actor, requires nullcheck by implementation.
	AActor* GetBlockingActor();
private:
	// Radius of capsule for SweepTrace from camera to player.
	UPROPERTY(EditAnywhere)
	float CapsuleRadius = 50.f;

	// Half height of capsule for SweepTrace from camera to player.
	UPROPERTY(EditAnywhere)
	float CapsuleHalfHeight = 100.f;
	
	float TransparencyDegree;
	AActor* BlockingActor;
	
	void SetTransparency(const FHitResult& HitResult, const FVector TraceStart, const FVector );
};
