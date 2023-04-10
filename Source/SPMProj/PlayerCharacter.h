// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SPMPROJ_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

private:
	//Show rotation speed in the Editor, Define value in BP inspector
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 10;
	//Show Movement speed in the Editor, Define value in BP inspector
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 10000;

	//Show Enhanced Inputs in the Editor, Define all inputs + inputmapping in BP inspector
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext *inputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputMoveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputMoveRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputLookUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputLookRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction *inputLookRightRate;

	//callback functions for Input actions
	void MoveForward(const FInputActionValue &Value);
	void MoveRight(const FInputActionValue &Value);
	void LookUp(const FInputActionValue &Value);
	void LookUpRate(const FInputActionValue &Value);
	void LookRight(const FInputActionValue &Value);
	void LookRightRate(const FInputActionValue &Value);
};
