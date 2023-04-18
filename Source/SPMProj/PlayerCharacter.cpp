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
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SavedGame.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	PlayerEIComponent->BindAction(InputDodge, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
	PlayerEIComponent->BindAction(InputTargetLock, ETriggerEvent::Triggered, this, &APlayerCharacter::TargetLock);
	//Testinputs för load och save
	PlayerEIComponent->BindAction(InputSaveGame, ETriggerEvent::Triggered, this, &APlayerCharacter::SaveGame);
	PlayerEIComponent->BindAction(InputLoadGame, ETriggerEvent::Triggered, this, &APlayerCharacter::LoadGame);

}

void APlayerCharacter::SetWeaponCollison(ECollisionEnabled::Type Collision)
{
	UE_LOG(LogTemp, Warning, TEXT("SetWeaponCollision ENTERED BEFORE IF"))
	if(EquipedWeapon && EquipedWeapon->GetCollisionBox())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWeaponCollision ENTERED IN IF"))
		EquipedWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
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
	if(ActionState == ECharacterActionState::ECAS_NoAction)
		AddControllerYawInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(const FInputActionValue &Value)
{
	if(ActionState == ECharacterActionState::ECAS_NoAction)
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
		WeaponState = ECharacterWeaponState::ECWS_Equiped;
	}

	
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	FHitResult HitResult;
	FVector TraceLocStart;
	FRotator TraceRot;
	PlayerController->GetPlayerViewPoint(TraceLocStart, TraceRot);
	FVector TraceLocEnd = TraceLocStart + TraceRot.Vector() * InteractableReach;
	bool bHitSucceed = GetWorld()->LineTraceSingleByChannel(HitResult, TraceLocStart, TraceLocEnd, ECC_GameTraceChannel1);

	if (bHitSucceed)
	{
		UE_LOG(LogTemp, Display, TEXT("Hit Interactable Actor: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		UActorComponent* HitActorComp = HitResult.GetActor()->GetComponentByClass(InteractableClass);
		if (HitActorComp == nullptr) return;
		UInteractableComponent* HitInteractableComp = Cast<UInteractableComponent>(HitActorComp);
		if (HitInteractableComp == nullptr) return;
		HitInteractableComp->Interact(this);
	}
	/*
		if (InteractableActor == nullptr) return;
		UInteractableComponent* InteractableComponent = Cast<UInteractableComponent>(InteractableActor->GetComponentByClass(InteractableClass));
		InteractableComponent->Interact(this);
		UE_LOG(LogTemp, Display, TEXT("Player interact with: %s Actor of class: %s"), *InteractableActor->GetActorNameOrLabel(), *InteractableClass->GetFullName()); 
	 */
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
	//...
}

void APlayerCharacter::TargetLock(const FInputActionValue& Value)
{
	if(!EnemyTargetLock)
		EnemyTargetLock = nullptr;

	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	FHitResult Hit;
	FVector TraceStart;
	FRotator TraceRot;
	PlayerController->GetPlayerViewPoint(TraceStart, TraceRot);
	FVector TraceEnd = TraceStart + TraceRot.Vector() * TargetLockDistance;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		TraceStart, TraceEnd,
		100.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true);

	if(IsValid(Hit.GetActor()))
	{
		EnemyTargetLock = Cast<AEnemy>(Hit.GetActor());
	}
}

void APlayerCharacter::KeepRotationOnTarget()
{
	if(!IsValid(EnemyTargetLock))
	{
		EnemyTargetLock = nullptr;
		return;
	}
	
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;

	if(EnemyTargetLock)
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyTargetLock->GetActorLocation());
		//FRotator OffSet = FRotator(40.f, 0.f, 0.f);
		PlayerController->SetControlRotation(NewRotation);
	}
}

void APlayerCharacter::PlayNormalAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
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
	return ActionState == ECharacterActionState::ECAS_NoAction && WeaponState == ECharacterWeaponState::ECWS_Equiped;
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

void APlayerCharacter::SaveGame()
{
	//Create instance of SavedGame
	USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//Set save game instance location to players current location
	SaveGameInstance->PlayerPosition = this->GetActorLocation();
	//save game instance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);


	//log message to show saved game
	UE_LOG(LogTemp, Display, TEXT("SAVED"));
}

void APlayerCharacter::LoadGame()
{	
	//Create instance of save game
	USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//Load saved game into instance variable
	SaveGameInstance = Cast<USavedGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	//set players position from saved position
	this->SetActorLocation(SaveGameInstance->PlayerPosition);


	//log to check for load
	UE_LOG(LogTemp, Display, TEXT("Loaded"));
}