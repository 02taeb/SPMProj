// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SPMPROJ_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	//Test för inventory, ger player ett inventorycomponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = true))
	class UInventoryComponent* Inventory;

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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Functions to enable or disable weapon box collison in blueprints*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollison(ECollisionEnabled::Type Collision);

	UPROPERTY(EditDefaultsOnly, Category = "Interacting")
	TSubclassOf<class UInteractableComponent> InteractableClass;

	//En Health för item att heala, kan ändras till annat health system sen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;
	//Gör att spelaren kan använda item
	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class AItemActor* Item);


private:
	//Show rotation speed in the Editor, Define value in BP inspector
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 10;
	//Show Movement speed in the Editor, Define value in BP inspector
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 10000;
	// Time jumpforce is applied for
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
    float JumpTime = 0.1;
	
	// Hur långt från KAMERAN som spelaren kan interagera med objekt.
	UPROPERTY(EditDefaultsOnly, Category = "Interacting")
	float InteractableReach = 500;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* OverlapWeapon;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* EquipedWeapon;

	/*Spelaren börjar unequiped*/
	ECharacterWeaponState WeaponState = ECharacterWeaponState::ECWS_Unequiped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_NoAction;

	/*Animation montage for basic attack*/
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* NormalAttackMontage;

	/*/*Animation montage for dodge#1# 
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* DodgeMontage;*/
	/*
	UPROPERTY(EditAnywhere, Category = "Interacting")
	TSoftObjectPtr<AActor> InteractableActor;
	*/
	
	//https://community.gamedev.tv/t/how-to-for-ue5-1-enhanced-input-system/221375?_gl=1*kvhzsk*_ga*MTc5MTAzNDQ2Mi4xNjgwMTk0MTc2*_ga_2C81L26GR9*MTY4MTAwODIzNC4yNS4xLjE2ODEwMDgyNjIuMC4wLjA
	//Show Enhanced Inputs in the Editor, Define all inputs + inputmapping in BP inspector
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMoveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMoveRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputLookUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputLookRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputInteract;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputLookRightRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAttackMeleeNormal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* InputJump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputDodge;

	
	//callback functions for Input actions
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void LookUpRate(const FInputActionValue &Value);
	void LookRight(const FInputActionValue& Value);
	void LookRightRate(const FInputActionValue &Value);
	void Interact(const FInputActionValue& Value);
	void AttackMeleeNormal(const FInputActionValue& Value);
	void JumpChar(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);

	void PlayAttackAnimation();
	
	/*Kollar om States uppfyller kravet för att kunna attackera*/
	bool CanAttack();

public:
	/*Setter for MeleeWeapon class, BeginOverlap sets the weapon pointer to MeleeWeapon object, EndOverlap setts the weapon to nullptr
	 * Här i public längst under för att vi har en forward deklaration uppe.
	 */ 
	FORCEINLINE void SetOverlapWeapon(AMeleeWeapon* Weapon) { OverlapWeapon = Weapon; }
	/*Dumb fucking function, tried too access Player via default Animation bluepring but didnt work... Remove in future*/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE APlayerCharacter* GetPlayerThis() { return this; }
	
};
