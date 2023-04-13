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
	TraceLocStart += 10 * TraceRot.Vector();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	bool bHitSucceed = GetWorld()->SweepSingleByChannel(HitResult, TraceLocStart, TraceLocEnd, TraceRot.Quaternion(), ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), Params);
	// Actual camera location
	//DrawDebugCamera(GetWorld(), GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation(), TraceRot, 90, 1, FColor::Yellow, false, 10.f);
	if (bHitSucceed)
	{
		// Sphere at start of Sweep trace
		//DrawDebugSphere(GetWorld(), TraceLocStart, SphereRadius, 10, FColor::Green, false, 10.f);
		// Sphere at impact point of sweep trace
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 10, FColor::Red, false, 10.f);
		// Camera at start of Sweep trace
		//DrawDebugCamera(GetWorld(), TraceLocStart, TraceRot, 90, 1, FColor::Blue, false, 10.f);
		//UE_LOG(LogTemp, Display, TEXT("Hit Actor in front of camera: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		SetTransparency(HitResult, TraceLocStart, PlayerController->GetPawn()->GetActorLocation());
		BlockingActor = HitResult.GetActor();
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(BlockingActor->GetRootComponent());
		if (Mesh == nullptr) return;
		else
		{
			OnHit(GetBlockingActor(), GetTransparencyDegree());
		}
	}
	else
	{
		TransparencyDegree = 1;
		BlockingActor = nullptr;
	}
}

float UCameraTransparencyComponent::GetTransparencyDegree()
{
	return TransparencyDegree;
}

AActor* UCameraTransparencyComponent::GetBlockingActor()
{
	return BlockingActor;
}

void UCameraTransparencyComponent::SetTransparency(const FHitResult& HitResult, const FVector TraceStart, const FVector PlayerLoc)
{
	TransparencyDegree = FVector::Dist(HitResult.GetActor()->GetActorLocation(), TraceStart) / FVector::Dist(TraceStart, PlayerLoc);
}

