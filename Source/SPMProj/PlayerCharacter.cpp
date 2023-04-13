// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractableComponent.h"
#include "MeleeWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

		//TESTING FÖR INVENTORY
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;
	Health = 100.f;

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
}

void APlayerCharacter::SetWeaponCollison(ECollisionEnabled::Type Collision)
{
	if(OverlapWeapon && OverlapWeapon->GetCollisionBox())
	{
		OverlapWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
	}
}

void APlayerCharacter::MoveForward(const FInputActionValue & Value) {
	AddMovementInput(GetActorForwardVector() * Value.Get<float>());
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
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
	AddControllerYawInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(const FInputActionValue &Value)
{
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
	}
}

	//Använda det item som klickas på, finns möjlighet för c++ och blueprint
	//Är implementerad i blueprint just nu
void APlayerCharacter::UseItem(UItem *Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); //Blueprint event
	}
	
}