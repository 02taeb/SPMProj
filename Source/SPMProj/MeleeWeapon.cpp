// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeleeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee Weapon Mesh"));
	RootComponent = MeleeMesh;
	
	PickupZone = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-up zone"));
	PickupZone->SetupAttachment(GetRootComponent());

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());

	BTStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Point"));
	BTStart->SetupAttachment(GetRootComponent());

	BTEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Point"));
	BTEnd->SetupAttachment(GetRootComponent());
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	/*Binding the callback function to the delegate*/
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBoxOverlap);
}

void AMeleeWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*Empty Actor* Array. Sends the array of actors to ignore tracing against (empty for now)*/
	TArray<AActor*> ActorsToIgnore;
	/*Hit result out parametar*/
	FHitResult BoxHit;
	
	UKismetSystemLibrary::BoxTraceSingle(
	this, 
	BTStart->GetComponentLocation(),     /*Getting the world location of the start and end trace points*/
	BTEnd->GetComponentLocation(),
	FVector(3.5f, 3.5f, 3.5f),   /*Size of trace box*/
	BTStart->GetComponentRotation(),   /*Box trace orientation reference component, taking start*/
	ETraceTypeQuery::TraceTypeQuery1,  
	false,   /*Traces only against simple collision*/
	ActorsToIgnore,
	EDrawDebugTrace::ForDuration,  /*Debug Sphere on ImpactPoint*/
	BoxHit,  
	true );  /*Ignores itself for overlaps*/

	if(GEngine)
	{
		FString HitObject = BoxHit.GetActor()->GetActorNameOrLabel();
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, HitObject);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Weapon hit result: %s"), *BoxHit.GetActor()->GetActorNameOrLabel());
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

