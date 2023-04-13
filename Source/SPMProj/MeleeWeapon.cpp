// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeleeWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee Weapon Mesh"));
	RootComponent = MeleeWeaponMesh;
	
	PickupZone = CreateDefaultSubobject<USphereComponent>(TEXT("Pick-up zone"));
	PickupZone->SetupAttachment(GetRootComponent());

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); /*Disable collision as it should only be enable under the attack*/
	
	BTStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Point"));
	BTStart->SetupAttachment(GetRootComponent());

	BTEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Point"));
	BTEnd->SetupAttachment(GetRootComponent());
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	/*Binding the callback function to the delegate*/
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBoxOverlap);
	PickupZone->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnSphereBeginOverlap);
	PickupZone->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeapon::OnSphereEndOverlap);
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

	if(IsValid(BoxHit.GetActor()))
	{
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), DefaultDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
	
}

void AMeleeWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->SetOverlapWeapon(this);
	}
}

void AMeleeWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->SetOverlapWeapon(nullptr);
	}
}

UBoxComponent* AMeleeWeapon::GetCollisionBox() const
{
	return CollisionBox;
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeapon::AttachWeaponOnPlayer(USceneComponent* Player, FName SocketLabel)
{
	MeleeWeaponMesh->AttachToComponent(Player, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketLabel);
}

