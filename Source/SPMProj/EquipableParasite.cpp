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

	// Set PlayerPtr
	//PlayerActorPtr = GetWorld()->GetFirstPlayerController()->GetOwner();
	
	// It would probably be okay to set them already here instead of waiting for pickup
}
	
void AEquipableParasite::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove buffs from player
	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(-StartAmount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(-StartAmount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(-StartAmount);
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

	//PlayerActorPtr = GetWorld()->GetFirstPlayerController()->GetOwner();

	

	if (PlayerActorPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("NULL"));
	}
	

	// Set statcomponentptr
	if (PlayerActorPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("NULL PLAYERPOINTER"));
	}

	StatComponentPtr = Cast<UStatComponent>(PlayerActorPtr->GetComponentByClass(UStatComponent::StaticClass()));
	
	if (StatComponentPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("nullptr"));
	}
	else{
		UE_LOG(LogTemp, Display, TEXT("not null"));
	}

	// Allow equipping
	bCanEquip = true;
}

void AEquipableParasite::OnEquip()
{
	if (Stat == EAffectedStat::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unset stat type for equipping parasite: %s"), *GetActorNameOrLabel());
		if (StatComponentPtr == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("NULL"));
		}
		
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
		StatComponentPtr->IncreaseMaxHealth(StartAmount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(StartAmount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(StartAmount);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for equipping parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}
	
	// Register as equipped
	// Allow unequipping
	bIsEquipped = true;
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
		StatComponentPtr->IncreaseMaxHealth(-StartAmount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(-StartAmount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(-StartAmount);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for equipping parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}

	bIsEquipped = false;
}

void AEquipableParasite::OnPlayerDeath()
{
	//TODO: Kalla på den här metoden när spelaren dör
	// Destroy this

	//Kommenterade bort då jag tror Destroy kommer skapa problem då pointers i inventory kommer vara null, kanske borde göras genom remove item istället
	// Destroy();
}

void AEquipableParasite::OnEat()
{
	UE_LOG(LogTemp, Display, TEXT("Reached Parasites OnEat"));

	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(OnEatUpgradeAmount);
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(OnEatUpgradeAmount);
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(OnEatUpgradeAmount);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for upgrading parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}
}

void AEquipableParasite::Use(APlayerCharacter* Character)
{
	if (bIsEquipped)
	{
		OnUnequip();
	}
	else
	{
		OnEquip();
	}
}

