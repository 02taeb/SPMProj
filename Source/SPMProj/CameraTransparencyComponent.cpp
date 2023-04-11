// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTransparencyComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCameraTransparencyComponent::UCameraTransparencyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraTransparencyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCameraTransparencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	FHitResult HitResult;
	FVector TraceLocStart;
	FRotator TraceRot;
	PlayerController->GetPlayerViewPoint(TraceLocStart, TraceRot);
	FVector TraceLocEnd = GetOwner()->GetActorLocation();
	
	bool bHitSucceed = GetWorld()->SweepSingleByChannel(HitResult, TraceLocStart, TraceLocEnd, TraceRot.Quaternion(), ECC_Visibility, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight));
	DrawDebugCapsule(GetWorld(), HitResult.ImpactPoint, CapsuleHalfHeight, CapsuleRadius, TraceRot.Quaternion(), FColor::Red, false, 10.f);
	if (bHitSucceed)
	{
		UE_LOG(LogTemp, Display, TEXT("Hit Actor in front of camer: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		SetTransparency(HitResult, TraceLocStart, PlayerController->GetPawn()->GetActorLocation());
		BlockingActor = HitResult.GetActor();
	}
	else
	{
		TransparencyDegree = 1;
		BlockingActor = nullptr;
	}
}

void UCameraTransparencyComponent::SetTransparency(const FHitResult& HitResult, const FVector TraceStart, const FVector PlayerLoc)
{
	TransparencyDegree = FVector::Dist(HitResult.GetActor()->GetActorLocation(), TraceStart) / FVector::Dist(TraceStart, PlayerLoc);
}

