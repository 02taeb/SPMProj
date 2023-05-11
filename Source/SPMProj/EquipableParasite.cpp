// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableParasite.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "StatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
}
	
void AEquipableParasite::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (StatComponentPtr == nullptr)
	{
		return;
	}
	
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

	
	if (System && System->IsActive())
		System->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation());
	
}

void AEquipableParasite::OnPickup()
{
	// Hide object in world
	StaticMeshComponent->SetVisibility(false);

	// Set statcomponentptr
	if (PlayerActorPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Null PlayerActorPointer"));
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
			UE_LOG(LogTemp, Display, TEXT("StatComponentPtr is null"));
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

	if (Particles && !System)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Particles!"));
		FVector Loc = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
		Loc.Z -= 100;
		System = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Particles,
			UGameplayStatics::GetPlayerCharacter(this, 0)->GetMesh(),
			TEXT("ball_rSocket"),
			Loc,
			FRotator(0),
			EAttachLocation::KeepWorldPosition,
			false);
	}
	else if(System)
		System->Activate();
	
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
	Equipped = true;
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

	if(Particles && System)
		System->Deactivate();

	switch (Stat)
	{
	case EAffectedStat::Health:
		StatComponentPtr->IncreaseMaxHealth(-(StartAmount + OnEatUpgradeAmount * TimesUpgraded));
		break;
	case EAffectedStat::Armor:
		StatComponentPtr->IncreaseArmor(-(StartAmount + OnEatUpgradeAmount * TimesUpgraded));
		break;
	case EAffectedStat::AttackDamage:
		StatComponentPtr->IncreaseAttackDamage(-(StartAmount + OnEatUpgradeAmount * TimesUpgraded));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognised stat for equipping parasite: %s"),
			*GetActorNameOrLabel());
		break;
	}

	bIsEquipped = false;
	Equipped = false;
}

void AEquipableParasite::OnPlayerDeath()
{
	//TODO: Kalla på den här metoden när spelaren dör
	// Destroy this
	if (bIsEquipped)
		OnUnequip();
	//Kommenterade bort då jag tror Destroy kommer skapa problem då pointers i inventory kommer vara null, kanske borde göras genom remove item istället
	// Destroy();
}

void AEquipableParasite::OnEat()
{
	UE_LOG(LogTemp, Display, TEXT("Reached Parasites OnEat"));
	TimesUpgraded++;
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

