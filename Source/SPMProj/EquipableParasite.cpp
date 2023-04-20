// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableParasite.h"

#include "StatComponent.h"

// Sets default values
AEquipableParasite::AEquipableParasite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AEquipableParasite::BeginPlay()
{
	Super::BeginPlay();

	//TODO: Fråga Hugo om något behövs göras här? Eller om något behövs i blueprint.

	// Set PlayerPtr
	PlayerActorPtr = GetWorld()->GetFirstPlayerController()->GetOwner();

	// Set statcomponentptr
	StatComponentPtr = Cast<UStatComponent>(PlayerActorPtr->GetComponentByClass(TSubclassOf<UStatComponent>()));

	// It would probably be okay to set them already here instead of waiting for pickup
}
	
void AEquipableParasite::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove buffs from player
	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(-Amount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(-Amount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(-Amount);
		break;
	case EAffectedStat::None:
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised or unset stat for dying parasite: %s"), *GetActorNameOrLabel());
		break;
	}
}

// Called every frame
void AEquipableParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipableParasite::OnPickup()
{
	// Hide object in world
	if(bUseStaticMesh) StaticMeshComponent->SetVisibility(false);

	// Add to inventory
	//TODO: Fråga Hugo om vad som behövs för att lägga till i inventory

	// Allow equipping
	bCanEquip = true;
}

void AEquipableParasite::OnEquip()
{
	if (Stat == EAffectedStat::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unset stat type for equipping parasite: %s"), *GetActorNameOrLabel());
		return;
	}
	if (!bCanEquip || bIsEquipped) return;

	if(bUseStaticMesh)
	{
		// Attach to socket on player
		StaticMeshComponent->SetVisibility(true);
		StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->AttachToComponent(
			Cast<USceneComponent>(PlayerActorPtr->GetComponentByClass(
				TSubclassOf<USkeletalMeshComponent>())),
			FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("ParasiteSocket"));
	}

	// Give buff to player
	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(Amount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(Amount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(Amount);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for equipping parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}
	
	// Register as equipped
	// Allow unequipping
	bIsEquipped = true;
	
	// Remove from inventory? Displayed on UI? Changed appearance in inventory to mark as equipped?
	//TODO: Fråga Hugo vad som bör göras när ett item blir equipped.
}

void AEquipableParasite::OnUnequip()
{
	if (!bIsEquipped) return;

	if(bUseStaticMesh)
	{
		// Reverse of OnEquip()
		StaticMeshComponent->SetVisibility(false);
		StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(-Amount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(-Amount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(-Amount);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for equipping parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}

	bIsEquipped = false;
	
	// Remove from inventory? Displayed on UI? Changed appearance in inventory to mark as equipped?
	//TODO: Fråga Hugo vad som bör göras när ett item blir unequipped.
}

void AEquipableParasite::OnPlayerDeath()
{
	// Destroy this
	Destroy();
}

