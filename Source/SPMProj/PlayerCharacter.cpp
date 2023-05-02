// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Enemy.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractableComponent.h"
#include "MeleeWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"
#include "ItemActor.h"
#include "EquipableItemActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "SavedGame.h"
#include "StatComponent.h"
#include "Components/SphereComponent.h"
#include "EquipableParasite.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Stats*/
	Stats = CreateDefaultSubobject<UStatComponent>("Stats");

		//TESTING FÖR INVENTORY
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;
	
	Health = 100.f;
	
	bHeavyAttackUsed = false;
	HeavyAttackCooldown = 7.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Display, TEXT("Char spawned"));
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp) MovementComp->MaxWalkSpeed = MovementSpeed; // Set the max walking speed here

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KeepRotationOnTarget();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Get controller
	auto PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr) return;
	
    // Get the local player enhanced input subsystem
    auto EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (EISubsystem == nullptr) return;
	
    //Add the input mapping context
    EISubsystem->AddMappingContext(InputMapping, 0);
 
    // Get the EnhancedInputComponent
    auto PlayerEIComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerEIComponent == nullptr) return;
	
	// Enhanced inputs tar bara input actions, därmed behöver man bara använda BindAction (inte BindAxis etc)
	PlayerEIComponent->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
	PlayerEIComponent->BindAction(InputMoveRight, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	PlayerEIComponent->BindAction(InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
	PlayerEIComponent->BindAction(InputLookUpRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUpRate);
	PlayerEIComponent->BindAction(InputLookRight, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRight);
	PlayerEIComponent->BindAction(InputLookRightRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRightRate);
	PlayerEIComponent->BindAction(InputInteract, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	PlayerEIComponent->BindAction(InputAttackMeleeNormal, ETriggerEvent::Triggered, this, &APlayerCharacter::AttackMeleeNormal);
	PlayerEIComponent->BindAction(InputAttackMeleeHeavy, ETriggerEvent::Triggered, this, &APlayerCharacter::AttackMeleeHeavy);
	PlayerEIComponent->BindAction(InputJump, ETriggerEvent::Started, this, &APlayerCharacter::JumpChar);
	PlayerEIComponent->BindAction(InputDodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
	PlayerEIComponent->BindAction(InputTargetLock, ETriggerEvent::Started, this, &APlayerCharacter::TargetLock);
	//Testinputs för load och save
	PlayerEIComponent->BindAction(InputSaveGame, ETriggerEvent::Started, this, &APlayerCharacter::SaveGame);
	PlayerEIComponent->BindAction(InputLoadGame, ETriggerEvent::Started, this, &APlayerCharacter::LoadGame);

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYER HAS TAKEN DAMAGE"));

	//Temporär damage + death
	Health = Health - 25;
	if (Health <= 0)
	{
		Destroy();
	}
	
	if(Stats)
	{
		Stats->TakeDamage(DamageAmount);
		if(Stats->Dead())
		{
			/*Död Logiken hör (respawn och sånt)*/
			UE_LOG(LogTemp, Warning, TEXT("PLAYER SHOULD DIE"));
		}
	}
	
	return DamageAmount;
}

void APlayerCharacter::SetWeaponCollison(ECollisionEnabled::Type Collision)
{
	if(EquipedWeapon && EquipedWeapon->GetCollisionBox())
	{
		EquipedWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
		EquipedWeapon->ActorsToIgnore.Empty();
	}
}

void APlayerCharacter::MoveForward(const FInputActionValue & Value)
{
	/*Cant move under attack, will be changed!!*/
	if(ActionState == ECharacterActionState::ECAS_NoAction)
		AddMovementInput(GetActorForwardVector() * Value.Get<float>());
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	/*Cant move under attack, will be changed!!*/
	if(ActionState == ECharacterActionState::ECAS_NoAction)
		AddMovementInput(GetActorRightVector() * Value.Get<float>());
}

void APlayerCharacter::LookUp(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpRate(const FInputActionValue &Value)
{
	AddControllerPitchInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRight(const FInputActionValue& Value)
{
	if(ActionState != ECharacterActionState::ECAS_AttackingNormal && ActionState != ECharacterActionState::ECAS_AttackingHeavy)
		AddControllerYawInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(const FInputActionValue &Value)
{
	if(ActionState != ECharacterActionState::ECAS_AttackingNormal && ActionState != ECharacterActionState::ECAS_AttackingHeavy)
		AddControllerYawInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{

	/*Equip part, just a first test place*/
	if(AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(OverlapWeapon))
	{
		Weapon->AttachWeaponOnPlayer(GetMesh(), FName("RightHandWeaponSocket"));
		/*Association the Player attaching the weapon to the weapon itself, so that we can get the instigator controller in ApplyDamage. GetInstigator() and GetOwner() return Player now when called from Weapon class if it is attached*/
		Weapon->SetOwner(this);
		Weapon->SetInstigator(this);
		EquipedWeapon = Weapon;
		OverlapWeapon = nullptr;
		Weapon->GetComponentByClass(USphereComponent::StaticClass())->DestroyComponent();
		WeaponState = ECharacterWeaponState::ECWS_Equiped;
	}

	
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	FHitResult HitResult;
	FVector TraceLocStart = GetActorLocation();
	FVector TraceLocEnd = TraceLocStart + GetActorForwardVector() * InteractableReach;
	bool bHitSucceed = GetWorld()->SweepSingleByChannel(HitResult, TraceLocStart, TraceLocEnd, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(34, 100));

	if (bHitSucceed)
	{
		UE_LOG(LogTemp, Display, TEXT("Hit Interactable Actor: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		UActorComponent* HitActorComp = HitResult.GetActor()->GetComponentByClass(InteractableClass);
		if (HitActorComp == nullptr) return;
		UInteractableComponent* HitInteractableComp = Cast<UInteractableComponent>(HitActorComp);
		if (HitInteractableComp == nullptr) return;
		HitInteractableComp->Interact(this);
	}
}

void APlayerCharacter::AttackMeleeNormal(const FInputActionValue& Value)
{
	if(CanAttack())
	{
		ActionState = ECharacterActionState::ECAS_AttackingNormal;
		PlayNormalAttackAnimation();
	}
}

void APlayerCharacter::AttackMeleeHeavy(const FInputActionValue& Value)
{
	if(CanAttack() && !bHeavyAttackUsed)
	{
		bHeavyAttackUsed = true;
		ActionState = ECharacterActionState::ECAS_AttackingHeavy;
		PlayHeavyAttackAnimation();
		GetWorld()->GetTimerManager().SetTimer(HeavyAttackTimer, this, &APlayerCharacter::ResetHeavyAttackCooldown, HeavyAttackCooldown, false); //HeavyAttackMontage->GetPlayLength()
	}
}

void APlayerCharacter::ResetHeavyAttackCooldown()
{
	bHeavyAttackUsed = false;
}

void APlayerCharacter::JumpChar(const FInputActionValue& Value)
{
	Super::Jump();
	JumpMaxHoldTime = JumpTime;

	FTimerHandle PlayerStopJumpingHandle;
	FTimerDelegate PlayerStopJumpingDelegate = FTimerDelegate::CreateUObject(this, &Super::StopJumping);
	GetWorldTimerManager().SetTimer(PlayerStopJumpingHandle, PlayerStopJumpingDelegate, JumpTime, false);
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
	if(GetCharacterMovement()->GetLastInputVector() == FVector::ZeroVector) return;
	if(ActionState != ECharacterActionState::ECAS_NoAction) return;
	
	ActionState = ECharacterActionState::ECAS_Dodging;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
		FVector PlayerVelocity = GetVelocity();

		FVector ForwardVector = GetActorForwardVector();
		FVector RightVector = GetActorRightVector();

		float ForwardDotProduct = FVector::DotProduct(PlayerVelocity, ForwardVector);
		float RightDotProduct = FVector::DotProduct(PlayerVelocity, RightVector);

		if (ForwardDotProduct > 0 && FMath::Abs(ForwardDotProduct) > FMath::Abs(RightDotProduct))
		{
			AnimInstance->Montage_JumpToSection(FName("DodgeForward"), DodgeMontage);
		}
		else if (ForwardDotProduct < 0 && FMath::Abs(ForwardDotProduct) > FMath::Abs(RightDotProduct))
		{
			AnimInstance->Montage_JumpToSection(FName("DodgeBackwards"), DodgeMontage);
		}
		else if (RightDotProduct > 0)
		{
			AnimInstance->Montage_JumpToSection(FName("DodgeRight"), DodgeMontage);
		}
		else if (RightDotProduct < 0)
		{
			AnimInstance->Montage_JumpToSection(FName("DodgeLeft"), DodgeMontage);
		}
	}
}

void APlayerCharacter::TargetLock(const FInputActionValue& Value)
{
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;
	
	TArray<FHitResult> HitResults;
	FVector TraceStart = GetActorLocation();
	FRotator TraceRot = GetActorRotation();
	FVector TraceEnd = TraceStart + TraceRot.Vector() * TargetLockDistance;
	TArray<AActor*> ActorsToIgnore;

	if(!EnemyTargetLock)
	{
		UKismetSystemLibrary::SphereTraceMulti(
		this,
		TraceStart,
		TraceEnd,
		60.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true);
	} else
	{
		EnemyTargetLock = nullptr;
		return;
	}

	for(auto Hit : HitResults)
	{
		if(IsValid(Hit.GetActor()) && Hit.GetActor()->IsA(AEnemy::StaticClass()))
		{
			EnemyTargetLock = Cast<AEnemy>(Hit.GetActor());
			break;
		}
	}
}

void APlayerCharacter::KeepRotationOnTarget()
{
	if(!IsValid(EnemyTargetLock))
		return;
	
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	if(EnemyTargetLock)
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTargetLock->GetActorLocation());
		FRotator Offset = FRotator(-15.f, 0.f, 0.f);
		PlayerController->SetControlRotation(NewRotation + Offset);
	}
}

void APlayerCharacter::PlayNormalAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
		const int32 RandomAnimation = FMath::RandRange(0, 1);
		FName AnimSection = FName();

		switch (RandomAnimation)
		{
		case 0:
			AnimSection = FName("BasicAttack1");
			break;
		case 1:
			AnimSection = FName("BasicAttack2");
			break;
		default:
			break;
		}
		
		AnimInstance->Montage_JumpToSection(AnimSection, NormalAttackMontage);
	}
}

void APlayerCharacter::PlayHeavyAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(HeavyAttackMontage);
	}
}

bool APlayerCharacter::CanAttack()
{
	return ActionState == ECharacterActionState::ECAS_NoAction && WeaponState == ECharacterWeaponState::ECWS_Equiped && ActionState != ECharacterActionState::ECAS_Dodging;
}

	//Använda det item som klickas på, finns möjlighet för c++ och blueprint
	//Är implementerad i blueprint just nu
void APlayerCharacter::UseItem(AItemActor *Item)
{
	if (Item)
	{
		if (Item->OwningInventory == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("No Owning Inventory"));
			return;
		}
		
		Item->Use(this);
		Item->OnUse(this); //Blueprint event
	}
	
}

void APlayerCharacter::OnEat()
{

	// Heala spelaren


	// Uppgradera equipped parasiter
	for (AItemActor* Item : Inventory->Items)
	{
		if (Cast<AEquipableParasite>(Item) && Cast<AEquipableParasite>(Item)->bIsEquipped == true)
		{
			Cast<AEquipableParasite>(Item)->OnEat();
		}
		
	}
	
}

void APlayerCharacter::SaveGame()
{
	//Create instance of SavedGame
	USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//Set save game instance location to players current location
	SaveGameInstance->PlayerPosition = this->GetActorLocation();

	//Reset equipped items
	SaveGameInstance->EquippedItems.Empty();

	//set current inventory to be saved
	SaveGameInstance->CurrentItems = Inventory->Items;

	for (AItemActor* Item : SaveGameInstance->CurrentItems)
	{
		if (Cast<AEquipableItemActor>(Item) && Cast<AEquipableItemActor>(Item)->Equipped)
		{
			SaveGameInstance->EquippedItems.Add(Cast<AEquipableItemActor>(Item));	
		}
		
	}
	

	//save game instance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);


	//log message to show saved game
	UE_LOG(LogTemp, Display, TEXT("SAVED"));
}

void APlayerCharacter::LoadGame()
{	
	//Create instance of save game
	USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//Load saved game into instance variable
	SaveGameInstance = Cast<USavedGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex));
	//set players position from saved position
	this->SetActorLocation(SaveGameInstance->PlayerPosition);

	//set inventory
	Inventory->Items.Empty();

	//Inventory->Items = SaveGameInstance->CurrentItems;
	

	

	for (AItemActor* Item : SaveGameInstance->CurrentItems)
	{
		if (Cast<AEquipableItemActor>(Item))
		{
			Cast<AEquipableItemActor>(Item)->Equipped = false;
		}
		
		Inventory->AddItem(Item);
	}

	for (AEquipableItemActor* Item : SaveGameInstance->EquippedItems)
	{
		Item->Equipped = true;
	}
	


	//log to check for load
	UE_LOG(LogTemp, Display, TEXT("Loaded"));
}