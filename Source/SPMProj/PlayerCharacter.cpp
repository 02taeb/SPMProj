// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
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
	//get controller
	auto playerController = Cast<APlayerController>(GetController());
	
    // Get the local player enhanced input subsystem
    auto eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
    //Add the input mapping context
    eiSubsystem->AddMappingContext(inputMapping, 0);
 
    // Get the EnhancedInputComponent
    auto playerEIcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//enhannced inputs tar bara input actions, därmed behöver man bara använda BindAction (inte BindAxis etc)
	playerEIcomponent->BindAction(inputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
	playerEIcomponent->BindAction(inputMoveRight, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	playerEIcomponent->BindAction(inputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
	playerEIcomponent->BindAction(inputLookUpRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUpRate);
	playerEIcomponent->BindAction(inputLookRight, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRight);
	playerEIcomponent->BindAction(inputLookRightRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRightRate);
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
