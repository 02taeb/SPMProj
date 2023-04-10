// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Char spawned"));
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
	PlayerEIComponent->BindAction(InputLookRight, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRight);
	PlayerEIComponent->BindAction(InputInteract, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::MoveForward(const FInputActionValue & Value) {
	//To bind to axis mapping: SetupPlayerInputComponent
	UE_LOG(LogTemp, Display, TEXT("Float value: %f"), Value.Get<float>());
	AddMovementInput(GetActorForwardVector() * Value.Get<float>());
	
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Float value: %f"), Value.Get<float>());
	AddMovementInput(GetActorRightVector() * Value.Get<float>());
}

void APlayerCharacter::LookUp(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Looking up Float value: %f"), Value.Get<float>());
	AddControllerPitchInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("looking right Float value: %f"), Value.Get<float>());
	AddControllerYawInput(Value.Get<float>() * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractableActor == nullptr) return;
	UInteractableComponent* InteractableComponent = Cast<UInteractableComponent>(InteractableActor->GetComponentByClass(InteractableClass));
	InteractableComponent->Interact(this);
	UE_LOG(LogTemp, Display, TEXT("Player interact with: %s Actor of class: %s"), *InteractableActor->GetActorNameOrLabel(), *InteractableClass->GetFullName());
}
