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
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Stats*/
	Stats = CreateDefaultSubobject<UStatComponent>("Stats");

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");

		//TESTING FÖR INVENTORY
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;
	
	
	// Health = 100.f;
	
	/*bHeavyAttackUsed = false;
	HeavyAttackCooldown = 7.0f;*/
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Display, TEXT("Char spawned"));
	MovementComp = GetCharacterMovement();
	if (MovementComp) MovementComp->MaxWalkSpeed = MovementSpeed; // Set the max walking speed here

	RespawnPoint = GetActorLocation();
	GravityScale = MovementComp->GravityScale;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KeepRotationOnTarget();

	// Inc grav when falling
	MovementComp->GravityScale = GetVelocity().Z < 0 ? MovementComp->GravityScale + AddedGravityWhenFalling : GravityScale;
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

	/*Test*/
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::Forward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::Right);
	
	// Enhanced inputs tar bara input actions, därmed behöver man bara använda BindAction (inte BindAxis etc)
	PlayerEIComponent->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
	PlayerEIComponent->BindAction(InputMoveRight, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
	PlayerEIComponent->BindAction(InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
	PlayerEIComponent->BindAction(InputLookUpRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUpRate);
	PlayerEIComponent->BindAction(InputLookRight, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRight);
	PlayerEIComponent->BindAction(InputLookRightRate, ETriggerEvent::Triggered, this, &APlayerCharacter::LookRightRate);
	PlayerEIComponent->BindAction(InputInteract, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	PlayerEIComponent->BindAction(InputAttackMeleeNormal, ETriggerEvent::Started, this, &APlayerCharacter::AttackMeleeNormal);
	PlayerEIComponent->BindAction(InputAttackMeleeHeavy, ETriggerEvent::Started, this, &APlayerCharacter::AttackMeleeHeavy);
	PlayerEIComponent->BindAction(InputJump, ETriggerEvent::Started, this, &APlayerCharacter::JumpChar);
	PlayerEIComponent->BindAction(InputDodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
	PlayerEIComponent->BindAction(InputTargetLock, ETriggerEvent::Started, this, &APlayerCharacter::TargetLock);
	//Testinputs för load och save
	PlayerEIComponent->BindAction(InputSaveGame, ETriggerEvent::Started, this, &APlayerCharacter::SaveGame);
	PlayerEIComponent->BindAction(InputLoadGame, ETriggerEvent::Started, this, &APlayerCharacter::LoadGame);
	//Cheat inputs
	PlayerEIComponent->BindAction(InputGodMode, ETriggerEvent::Started, this, &APlayerCharacter::GodMode);
	PlayerEIComponent->BindAction(InputInstaKill, ETriggerEvent::Started, this, &APlayerCharacter::InstaKill);
	PlayerEIComponent->BindAction(InputNoClip, ETriggerEvent::Started, this, &APlayerCharacter::NoClip);
	PlayerEIComponent->BindAction(InputSpawnSword, ETriggerEvent::Started, this, &APlayerCharacter::SpawnSword);
	PlayerEIComponent->BindAction(InputTPFirst, ETriggerEvent::Started, this, &APlayerCharacter::TPFirst);
	PlayerEIComponent->BindAction(InputTPSecond, ETriggerEvent::Started, this, &APlayerCharacter::TPSecond);
	PlayerEIComponent->BindAction(InputTPThird, ETriggerEvent::Started, this, &APlayerCharacter::TPThird);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (ActionState == ECharacterActionState::ECAS_Dodging || bGodMode || bIsRespawning) return 0;
	UE_LOG(LogTemp, Warning, TEXT("PLAYER HAS TAKEN DAMAGE"));
	
	if(Stats)
	{
		Stats->TakeDamage(DamageAmount);
		
		PlaySound(TakeDamageSoundCue);

		if(Stats->Dead())
		{
			/*Död Logiken hör (respawn och sånt)*/
			PlaySound(DeathSoundCue);

			for (AItemActor* Item : Inventory->Items)
			{
				if (Cast<AEquipableParasite>(Item) && Cast<AEquipableParasite>(Item)->bIsEquipped == true)
				{
					//Cast<AEquipableParasite>(Item)->OnPlayerDeath();
					Inventory->RemoveItem(Item);

				}
			}
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->StopAllMontages(0.1f);
			}
			bIsRespawning = true;
			Stats->CurrentHealth = 0;
			this->GetMesh()->SetVisibility(false);
			this->GetMesh()->SetGenerateOverlapEvents(false);
			if (EquipedWeapon)
			{
				EquipedWeapon->MeleeWeaponMesh->SetVisibility(false);
			}
			//this->GetController()->UnPossess();
			DisableInput(Cast<APlayerController>(this->GetController()));
			//Destroy();
			FTimerHandle RespawnTimer;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &APlayerCharacter::Respawn,5);
			UE_LOG(LogTemp, Warning, TEXT("PLAYER SHOULD DIE"));
		} else
		{
			PlayPlayerHitReact();
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

void APlayerCharacter::Forward(float Value)
{
}

void APlayerCharacter::Right(float Value)
{
}

void APlayerCharacter::MoveForward(const FInputActionValue & Value)
{
	if (bNoClip)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Value.Get<float>() * NoClipSpeed);
		return;
	}
	
	/*Cant move under attack, will be changed!!*/
	if(ActionState == ECharacterActionState::ECAS_NoAction)
		AddMovementInput(GetActorForwardVector() * Value.Get<float>());
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	if (bNoClip)
	{
		SetActorLocation(GetActorLocation() + GetActorRightVector() * Value.Get<float>() * NoClipSpeed);
		return;
	}
	
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
		PlaySound(NormalAttackSoundCue);
		ActionState = ECharacterActionState::ECAS_AttackingNormal;
		PlayNormalAttackAnimation();
	}
}

void APlayerCharacter::AttackMeleeHeavy(const FInputActionValue& Value)
{
	if(CanAttack()) /*if(CanAttack() && !bHeavyAttackUsed)*/
	{
		///bHeavyAttackUsed = true;
		PlaySound(HeavyAttackSoundCue);
		ActionState = ECharacterActionState::ECAS_AttackingHeavy;
		PlayHeavyAttackAnimation();
		//GetWorld()->GetTimerManager().SetTimer(HeavyAttackTimer, this, &APlayerCharacter::ResetHeavyAttackCooldown, HeavyAttackCooldown, false); //HeavyAttackMontage->GetPlayLength()
	}
}

/*void APlayerCharacter::ResetHeavyAttackCooldown()
{
	bHeavyAttackUsed = false;
}*/

void APlayerCharacter::JumpChar(const FInputActionValue& Value)
{
	if(ActionState == ECharacterActionState::ECAS_Dodging) return;
	
	if (bNoClip)
	{
		SetActorLocation(GetActorLocation() + GetActorUpVector() * NoClipSpeed);
		return;
	}
	Super::Jump();
	PlaySound(JumpSoundCue);
	JumpMaxHoldTime = JumpTime;

	FTimerHandle PlayerStopJumpingHandle;
	FTimerDelegate PlayerStopJumpingDelegate = FTimerDelegate::CreateUObject(this, &Super::StopJumping);
	GetWorldTimerManager().SetTimer(PlayerStopJumpingHandle, PlayerStopJumpingDelegate, JumpTime, false);
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
	if(EquipedWeapon && EquipedWeapon->GetCollisionBox()->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		SetWeaponCollison(ECollisionEnabled::NoCollision);
	}
	
	if (bNoClip)
	{
		SetActorLocation(GetActorLocation() + GetActorUpVector() * -NoClipSpeed);
		return;
	}
	
	if(ActionState != ECharacterActionState::ECAS_AttackingNormal && ActionState != ECharacterActionState::ECAS_AttackingHeavy && ActionState != ECharacterActionState::ECAS_NoAction) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DodgeMontage)
	{
		auto PlayerEIComponent = FindComponentByClass<UInputComponent>();
		if (PlayerEIComponent == nullptr) return;
		
		float ForwardAxisValue = PlayerEIComponent->GetAxisValue("Forward");
		float RightAxisValue = PlayerEIComponent->GetAxisValue("Right");
		if(FMath::Abs(ForwardAxisValue) == 0 && FMath::Abs(RightAxisValue) == 0) return;

		AnimInstance->Montage_Play(DodgeMontage);
		PlaySound(RollSoundCue);
		ActionState = ECharacterActionState::ECAS_Dodging;

		if(FMath::Abs(ForwardAxisValue) == 1 && FMath::Abs(RightAxisValue) == 1)
		{
			if(ForwardAxisValue == 1 && RightAxisValue == -1) AnimInstance->Montage_JumpToSection(FName("DodgeFL"), DodgeMontage);
			if(ForwardAxisValue == 1 && RightAxisValue == 1) AnimInstance->Montage_JumpToSection(FName("DodgeFR"), DodgeMontage);
			if(ForwardAxisValue == -1 && RightAxisValue == -1) AnimInstance->Montage_JumpToSection(FName("DodgeBL"), DodgeMontage);
			if(ForwardAxisValue == -1 && RightAxisValue == 1) AnimInstance->Montage_JumpToSection(FName("DodgeBR"), DodgeMontage);
		} else
		{
			if (ForwardAxisValue > 0)
			{
				AnimInstance->Montage_JumpToSection(FName("DodgeForward"), DodgeMontage);
			}
			else if (ForwardAxisValue < 0)
			{
				AnimInstance->Montage_JumpToSection(FName("DodgeBackwards"), DodgeMontage);
			}
			else if (RightAxisValue > 0)
			{
				AnimInstance->Montage_JumpToSection(FName("DodgeRight"), DodgeMontage);
			}
			else if (RightAxisValue < 0)
			{
				AnimInstance->Montage_JumpToSection(FName("DodgeLeft"), DodgeMontage);
			}
		}
	}
}

void APlayerCharacter::TargetLock(const FInputActionValue& Value)
{
	AController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr) return;
	if(ActionState != ECharacterActionState::ECAS_NoAction) return;
	
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

void APlayerCharacter::GodMode(const FInputActionValue& Value)
{
	bGodMode = !bGodMode;
	UE_LOG(LogTemp, Display, TEXT("God mode: %d"), bGodMode);
}

void APlayerCharacter::InstaKill(const FInputActionValue& Value)
{
	bInstaKill = !bInstaKill;
	UE_LOG(LogTemp, Display, TEXT("Instakill: %d"), bInstaKill);
}

void APlayerCharacter::NoClip(const FInputActionValue& Value)
{
	bNoClip = !bNoClip;
	MovementComp -> GravityScale = bNoClip ? 0 : 1;
	SetActorEnableCollision(!bNoClip);
	UE_LOG(LogTemp, Display, TEXT("No clip: %d"), bNoClip);
}

void APlayerCharacter::SpawnSword(const FInputActionValue& Value)
{
	const FVector SpawnLoc = GetActorLocation();
	GetWorld()->SpawnActor(MeleeWeaponClass, &SpawnLoc);
	UE_LOG(LogTemp, Display, TEXT("Spawned Sword"));
}

void APlayerCharacter::TPFirst(const FInputActionValue& Value)
{
	SetActorLocation(TP1);
}

void APlayerCharacter::TPSecond(const FInputActionValue& Value)
{
	SetActorLocation(TP2);
}

void APlayerCharacter::TPThird(const FInputActionValue& Value)
{
	SetActorLocation(TP3);
}

void APlayerCharacter::SetRespawnPoint(FVector Position)
{
	RespawnPoint = Position;
}

void APlayerCharacter::Respawn()
{
	SetActorLocation(RespawnPoint);
	Stats->CurrentHealth = Stats->GetMaxHealth();
	this->GetMesh()->SetVisibility(true);
	this->GetMesh()->SetGenerateOverlapEvents(true);
	if (EquipedWeapon)
	{
		EquipedWeapon->MeleeWeaponMesh->SetVisibility(true);
	}
	EnableInput(Cast<APlayerController>(this->GetController()));
	bIsRespawning = false;
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
	//UAnimInstance* AnimInstance = MySkeletalMeshComponent->GetAnimInstance();
	
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
		//const int32 RandomAnimation = FMath::RandRange(0, 1);
		FName AnimSection = FName();

		switch (ComboIndex)
		{
		case 1:
			AnimSection = FName("BasicAttack1");
			ComboIndex++;
			break;
		case 2:
			AnimSection = FName("BasicAttack2");
			ComboIndex++;
			break;
		case 3:
			AnimSection = FName("BasicAttack3");
			ComboIndex = 1;
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

void APlayerCharacter::PlayCrouchAnimation()
{
	DisableInput(Cast<APlayerController>(Controller));
	ActionState = ECharacterActionState::ECAS_Crouching;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CrouchMontage)
		AnimInstance->Montage_Play(CrouchMontage);
	FTimerHandle StopEatingHandle;
	GetWorld()->GetTimerManager().SetTimer(StopEatingHandle, this, &APlayerCharacter::StopCrouch, 1);
}

void APlayerCharacter::StopCrouch()
{
	EnableInput(Cast<APlayerController>(Controller));
	ActionState = ECharacterActionState::ECAS_NoAction;
}


bool APlayerCharacter::CanAttack()
{
	return ActionState == ECharacterActionState::ECAS_NoAction && WeaponState == ECharacterWeaponState::ECWS_Equiped && ActionState != ECharacterActionState::ECAS_Dodging;
}

void APlayerCharacter::PlayPlayerHitReact()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		ActionState = ECharacterActionState::ECAS_IsHit;
		
		FName SectionToPlay = FName("HitStraight");

		if((HitAngle >= -45.f && HitAngle < 45.f) /*|| (HitAngle >= -135.f && HitAngle < 135.f)*/) /*From hit from front or from back (same animation for now)*/
			{
			SectionToPlay = FName("HitStraight");
			} else if(HitAngle >= -135.f && HitAngle < -45.f)
			{
				SectionToPlay = FName("HitFromLeft");
			} else if(HitAngle >= 45.f && HitAngle < 135.f)
			{
				SectionToPlay = FName("HitFromRight");
			}
		
		AnimInstance->Montage_JumpToSection(SectionToPlay, HitReactMontage);
	}
}

void APlayerCharacter::CalculateHitDirection(const FVector ImpactPoint)
{
	const FVector ForwardV = GetActorForwardVector();
	const FVector ToImpact = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(ForwardV, ToImpact);
	/*The angle between players forward vector and the vector from players location to the impact point (inverted cos), converted to degrees*/
	HitAngle = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	/*Checking if the Angle/Vector is negative of positive by using cross product, in order to get the exact direction of the hit*/
	/*If CrossProduct is positive, the player is hit from the right*/
	/*If CrossProduct is negative, the player is hit from the left*/
	const FVector CrossProduct = FVector::CrossProduct(ForwardV, ToImpact);
	if(CrossProduct.Z < 0) HitAngle *= -1.f;
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
		// Max two equipped parasites
		AEquipableParasite* EquippingPar = Cast<AEquipableParasite>(Item);
		if (EquippingPar != nullptr)
		{
			if (EquippedPar2 != nullptr && EquippedPar2 == EquippingPar)
				EquippedPar2 = nullptr;
			else if (EquippedPar1 != nullptr && EquippedPar1 == EquippingPar)
			{
				EquippedPar1 = EquippedPar2;
				EquippedPar2 = nullptr;
			}
			else if (EquippedPar1 == nullptr)
				EquippedPar1 = EquippingPar;
			else if (EquippedPar2 == nullptr)
				EquippedPar2 = EquippingPar;
			else
				return;
		}

		Item->Use(this);
		Item->OnUse(this); //Blueprint event
		if (EquippingPar != nullptr)
			OnEquipParasite(EquippedPar1, EquippedPar2);
	}
	
}

void APlayerCharacter::OnEat()
{

	PlaySound(EatingSoundCue);

	// Heala spelaren
	Stats->HealHealth(OnEatHealAmount);
	OnHeal();
	PlayCrouchAnimation();

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

void APlayerCharacter::PlaySound(USoundCue *Sound)
{
	if (AudioComponent && Sound)
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}
	
}
