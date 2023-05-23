// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "PlayerCharacter.generated.h"

//Hugo (det som behövs av de metoder jag gjort/jobbat med)


class AEquipableParasite;
class AMeleeWeapon;
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void CalculateHitDirection(const FVector ImpactPoint);

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
	UFUNCTION(BlueprintCallable)
	void OnEat();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Parasites")
	AActor* HPPar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Parasites")
	AActor* ATKPar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Parasites")
	AActor* DEFPar;
	
	bool bInstaKill = false;

	UFUNCTION(BlueprintCallable)
	void KillSelf();

	//Statcomponent, är public fär blueprint access
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	class UStatComponent* Stats;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHeal();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipParasite(AEquipableParasite* Par1, AEquipableParasite* Par2);
	UFUNCTION(BlueprintCallable)
	bool BothParSlotsFull(AEquipableParasite* UsingPar);
private:
	// Cheat vars
	UPROPERTY(EditAnywhere, Category = "Cheats")
	TSubclassOf<AMeleeWeapon> MeleeWeaponClass;
	UPROPERTY(EditAnywhere, Category = "Cheats")
	float NoClipSpeed = 50;
	UPROPERTY()
	UCharacterMovementComponent* MovementComp;
	bool bGodMode = false;
	bool bNoClip = false;
	UPROPERTY(EditAnywhere, Category = "Cheats")
	FVector TP1;
	UPROPERTY(EditAnywhere, Category = "Cheats")
	FVector TP2;
	UPROPERTY(EditAnywhere, Category = "Cheats")
	FVector TP3;

	float GravityScale;
	UPROPERTY(EditAnywhere, Category = "InputSpeeds")
	float AddedGravityWhenFalling = 0.5f;

	FTimerHandle StaminaTimer;

	// Deprecated
	UPROPERTY()
	AEquipableParasite* EquippedPar1 = nullptr;
	UPROPERTY()
	AEquipableParasite* EquippedPar2 = nullptr;
    
	//Audio
	UPROPERTY()
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* EatingSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* NormalAttackSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* HeavyAttackSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* JumpSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* TakeDamageSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* DeathSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundCue* RollSoundCue;

	// Respawning
	UFUNCTION(BlueprintCallable)
	void SetRespawnPoint(FVector Position, FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	FVector GetRespawnPoint();
	void Respawn();
	FVector RespawnPoint;
	FRotator RespawnRotation;
	bool bIsRespawning = false;
	
	//Function for saving and loading the game
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	void LoadGame();

	//Function for setting soundcue and playing sound
	void PlaySound(class USoundCue* Sound);

	//Show rotation speed in the Editor, Define value in BP inspector
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputSpeeds", meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float OnEatHealAmount = 25;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* OverlapWeapon;

	UPROPERTY(VisibleInstanceOnly)
	class AMeleeWeapon* EquipedWeapon;

	/*Spelaren börjar unequiped*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterWeaponState WeaponState = ECharacterWeaponState::ECWS_Unequiped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_NoAction;

	/*/*Cooldown variables#1#
	FTimerHandle HeavyAttackTimer;
	bool bHeavyAttackUsed;
	UPROPERTY(EditAnywhere, Category=Cooldown)
	float HeavyAttackCooldown;*/

	/*Target lock*/
	UPROPERTY()
	class AEnemy* EnemyTargetLock;
	UPROPERTY(EditAnywhere)
	float TargetLockDistance = 500.0f;

	/*Attack animation montages*/
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* NormalAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* HeavyAttackMontage;

	int32 ComboIndex = 1;

	/*Animation montage for dodge*/ 
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	class UAnimMontage* DodgeMontage;

	//Anims för ätande
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	UAnimMontage* CrouchMontage;

	/*Animation montage för hit react*/
	UPROPERTY(EditDefaultsOnly, Category=AnimationMontages)
	UAnimMontage* HitReactMontage;
	double HitAngle;
	
	/*UPROPERTY(EditAnywhere, Category = "Interacting")
	TSoftObjectPtr<AActor> InteractableActor;*/
	
	
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
	class UInputAction* InputAttackMeleeHeavy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* InputJump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputDodge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputTargetLock;

	//testinputs för save och load
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputSaveGame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputLoadGame;
	

	/*Test functions for dodge with old input system*/
	void Forward(float Value);
	void Right(float Value);

	//inputs for cheats
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputGodMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputInstaKill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputNoClip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputSpawnSword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputTPFirst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputTPSecond;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputTPThird;
	
	//callback functions for Input actions
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void LookUpRate(const FInputActionValue &Value);
	void LookRight(const FInputActionValue& Value);
	void LookRightRate(const FInputActionValue &Value);
	void Interact(const FInputActionValue& Value);
	void AttackMeleeNormal(const FInputActionValue& Value);
	void AttackMeleeHeavy(const FInputActionValue& Value);
	void JumpChar(const FInputActionValue& Value);
	void NoClipUp(const FInputActionValue& Value);
	void NoClipDown(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void TargetLock(const FInputActionValue& Value);

	void GodMode(const FInputActionValue& Value);
	void InstaKill(const FInputActionValue& Value);
	void NoClip(const FInputActionValue& Value);
	void SpawnSword(const FInputActionValue& Value);
	void TPFirst(const FInputActionValue& Value);
	void TPSecond(const FInputActionValue& Value);
	void TPThird(const FInputActionValue& Value);

	void KeepRotationOnTarget();

	void PlayNormalAttackAnimation();
	void PlayHeavyAttackAnimation();
	void PlayPlayerHitReact();
	void PlayCrouchAnimation();
	void StopCrouch();
	
	/*Kollar om States uppfyller kravet för att kunna attackera*/
	bool CanAttack();

	/*Cooldown metoder*/
	//void ResetHeavyAttackCooldown();
public:
	/*Setter for MeleeWeapon class, BeginOverlap sets the weapon pointer to MeleeWeapon object, EndOverlap setts the weapon to nullptr
	 * Här i public längst under för att vi har en forward deklaration uppe.
	 */ 
	FORCEINLINE void SetOverlapWeapon(AMeleeWeapon* Weapon) { OverlapWeapon = Weapon; }
	FORCEINLINE ECharacterActionState GetPlayerAttackType() { return ActionState; }
	
};
