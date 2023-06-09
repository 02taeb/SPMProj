// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "StatComponent.h"

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
	/*Sends the array of actors to ignore tracing against (Filled by ActorToIgnore Array)*/
	TArray<AActor*> Ignore;
	Ignore.Add(this->GetOwner());
	for(AActor* Actor : ActorsToIgnore)
	{
		Ignore.AddUnique(Actor);
	}
	
	FHitResult BoxHit;
	
	UBoxComponent* Box = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	FVector BoxSize = FVector(Box->Bounds.BoxExtent);
	UKismetSystemLibrary::BoxTraceSingle(
	this, 
	BTStart->GetComponentLocation(),     /*Getting the world location of the start and end trace points*/
	BTEnd->GetComponentLocation(),
	BoxSize,   /*Size of trace box*/
	BTStart->GetComponentRotation(),   /*Box trace orientation reference component, taking start*/
	ETraceTypeQuery::TraceTypeQuery1,  
	false,   /*Traces only against simple collision*/
	Ignore,
	EDrawDebugTrace::None,  /*Debug Sphere on ImpactPoint*/
	BoxHit,  
	true);  /*Ignores itself for overlaps*/
	
	HandleWeaponBoxHit(BoxHit.GetActor(), BoxHit.ImpactPoint, BoxHit);
}

void AMeleeWeapon::HandleWeaponBoxHit(AActor* Actor, const FVector ImpactPoint, FHitResult HitResult)
{
	if(!Actor) return;
	if(GetOwner()->ActorHasTag(FName("Enemy")) && Actor->ActorHasTag(FName("Enemy"))) return;
	
	OnHit(Actor, ImpactPoint, HitResult);
	ActorsToIgnore.AddUnique(Actor);
		
	if(GetOwner()->ActorHasTag(FName("Enemy")))
	{
		AEnemy* EnemyInstigator = Cast<AEnemy>(this->GetOwner());
		if(EnemyInstigator)
		{
			APlayerCharacter* HitPlayer = Cast<APlayerCharacter>(Actor);
			if(HitPlayer) HitPlayer->CalculateHitDirection(ImpactPoint);
			UGameplayStatics::ApplyDamage(Actor, DefaultDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			UE_LOG(LogTemp, Warning, TEXT("ENEMY"));
		}
	} else
	{
		APlayerCharacter* PlayerInstigator = Cast<APlayerCharacter>(this->GetOwner());
		if(PlayerInstigator && PlayerInstigator->GetPlayerAttackType() == ECharacterActionState::ECAS_AttackingNormal)
		{
			if (PlayerInstigator->bInstaKill)
			{
				UGameplayStatics::ApplyDamage(Actor, DefaultDamage * 9999, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			}
			else
			{
				AEnemy* HitEnemy = Cast<AEnemy>(Actor);
				if(HitEnemy) HitEnemy->CalculateHitDirection(ImpactPoint); /*Viktigt att kallas innan ApplyDamage!!*/
				float Damage = DefaultDamage;
				Damage += PlayerInstigator->Stats->GetCurrentAttackDamage();
				UGameplayStatics::ApplyDamage(Actor, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			}
			UE_LOG(LogTemp, Warning, TEXT("PLAYER DEF"));
		}
		else if(PlayerInstigator && PlayerInstigator->GetPlayerAttackType() == ECharacterActionState::ECAS_AttackingHeavy)
		{
			if (PlayerInstigator->bInstaKill)
			{
				UGameplayStatics::ApplyDamage(Actor, HeavyDamage * 9999, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			}
			else
			{
				AEnemy* HitEnemy = Cast<AEnemy>(Actor);
				if(HitEnemy) HitEnemy->CalculateHitDirection(ImpactPoint); /*Viktigt att kallas innan ApplyDamage!!*/
				UGameplayStatics::ApplyDamage(Actor, HeavyDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			}
			UE_LOG(LogTemp, Warning, TEXT("PLAYER HEV"));
		}
	}
	//är test för partikeleffekt när vapen träffar något
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, GetActorLocation());

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

