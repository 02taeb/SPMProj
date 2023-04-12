// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterTest.h"

#include "InteractableComponent.h"

// Sets default values
APlayerCharacterTest::APlayerCharacterTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacterTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &APlayerCharacterTest::Interact);
}

void APlayerCharacterTest::Interact()
{
	UInteractableComponent* InteractableComponent = Cast<UInteractableComponent>(InteractableActor->GetComponentByClass(InteractableClass));
	InteractableComponent->Interact(this);
}