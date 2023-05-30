// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "MeleeWeapon.h"
#include "PlayerCharacter.h"
#include "StatComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Components dont need to be attached.
	Stats = CreateDefaultSubobject<UStatComponent>("Stats");

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	/*Tag, mostly to check weapon collision*/
	Tags.Add(FName("Enemy"));

	if (GetWorld() && WeaponClass)
	{
		AMeleeWeapon* EquipedWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponClass);
		if (EquipedWeapon == nullptr) return;
		EquipedWeapon->AttachWeaponOnPlayer(GetMesh(), FName("RightHandWeaponSocket"));
		EquipedWeapon->SetOwner(this);
		EquipedWeapon->SetInstigator(this);
		if (EquipedWeapon->GetComponentByClass(USphereComponent::StaticClass()) == nullptr) return;
		EquipedWeapon->GetComponentByClass(USphereComponent::StaticClass())->DestroyComponent();
		EnemyWeapon = EquipedWeapon;
	}
}

void AEnemy::EnemyAttackBasic()
{
	PlayEnemyAttackMontage();
}

void AEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (EnemyWeapon == nullptr) return;
	EnemyWeapon->Destroy();
}

void AEnemy::PlayEnemyHitReact()
{
	if (GetMesh() == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EnemyHitReactMontage)
	{
		AnimInstance->Montage_Play(EnemyHitReactMontage);

		FName SectionToPlay = FName("HitStraight");

		if ((HitAngle >= -45.f && HitAngle < 45.f) /*|| (HitAngle >= -135.f && HitAngle < 135.f)*/)
		/*From hit from front or from back (same animation for now)*/
		{
			SectionToPlay = FName("HitStraight");
		}
		else if (HitAngle >= -135.f && HitAngle < -45.f)
		{
			SectionToPlay = FName("HitFromLeft");
		}
		else if (HitAngle >= 45.f && HitAngle < 135.f)
		{
			SectionToPlay = FName("HitFromRight");
		}

		AnimInstance->Montage_JumpToSection(SectionToPlay, EnemyHitReactMontage);
	}

	//set playerLocation if enemy has been hit by player after timer
	GetWorldTimerManager().SetTimer(SetPlayerLocationTimerAfterOnHit, this, &AEnemy::SetPlayerLocationAfterOnHit,
	                                TimeToReact, false);
}

//Daniel
void AEnemy::SetPlayerLocationAfterOnHit()
{
	UBlackboardComponent* Blackboard = nullptr;

	// Get the controller of the enemy actor
	AController* EnemyController = GetController();
	ensureMsgf(EnemyController != nullptr, TEXT("EnemyController is nullptr"));
	if (EnemyController)
	{
		// Try to cast the controller to an AAIController
		AAIController* AIController = Cast<AAIController>(EnemyController);
		ensureMsgf(AIController != nullptr, TEXT("AIController is nullptr"));
		if (AIController)
		{
			// Get the Blackboard component from the AI controller
			if (AIController->GetBlackboardComponent() == nullptr) return;
			Blackboard = AIController->GetBlackboardComponent();
		}
	}
	ensureMsgf(Blackboard != nullptr, TEXT("Blackboard is nullptr"));
	if (Blackboard)
	{
		//get player
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		ensureMsgf(Player != nullptr, TEXT("Player is nullptr"));
		if (Player != nullptr)
		{
			//set player location as a BB value
			Blackboard->SetValueAsVector("PlayerLocation", Player->GetActorLocation());
		}
	}
}


void AEnemy::CalculateHitDirection(const FVector ImpactPoint)
{
	const FVector ForwardV = GetActorForwardVector();
	const FVector ToImpact = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(ForwardV, ToImpact);
	/*The angle between enemys forward vector and the vector from enemys location to the impact point (inverted cos), converted to degrees*/
	HitAngle = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	/*Checking if the Angle/Vector is negative of positive by using cross product, in order to get the exact direction of the hit*/
	/*If CrossProduct is positive, the enemy is hit from the right*/
	/*If CrossProduct is negative, the enemy is hit from the left*/
	const FVector CrossProduct = FVector::CrossProduct(ForwardV, ToImpact);
	if (CrossProduct.Z < 0) HitAngle *= -1.f;
}


const bool AEnemy::GetIsAttacking()
{
	return IsAttacking;
}

UStatComponent* AEnemy::GetStats() const
{
	return Stats;
}

void AEnemy::SetWeaponCollison(ECollisionEnabled::Type Collision) const
{
	if (EnemyWeapon && EnemyWeapon->GetCollisionBox())
	{
		EnemyWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
		EnemyWeapon->ActorsToIgnore.Empty();
	}
}

void AEnemy::PlayEnemyAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	PlaySound(AttackSoundCue);

	if (AnimInstance && EnemyAttackMontage)
	{
		AnimInstance->Montage_Play(EnemyAttackMontage);

		FName SectionToPlay = FName();

		switch (AttackIndex)
		{
		case 1:
			SectionToPlay = FName("BasicFirst");
			AttackIndex++;
			break;
		case 2:
			SectionToPlay = FName("BasicSecond");
			AttackIndex = 1;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionToPlay, EnemyAttackMontage);

		AttackMontageDuration = EnemyAttackMontage->GetPlayLength() / 2;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAlongTargetLock();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/*The function is called via ApplyDamage, they communicate so to say...*/
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("ENEMY HAS TAKEN DAMAGE"));
	if (Stats)
	{
		PlaySound(TakeDamageSoundCue);
		OnHitBPEvent();
		Stats->TakeDamage(DamageAmount);
		if (Stats->Dead())
		{
			/*
			if (ActorToSpawn)
			{
				GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation() - FVector(0,0,100), GetActorRotation());
			}
			*/
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->StopAllMontages(0.1f);
			}
			PlaySound(DeathSoundCue);
			GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
			OnDeathBPEvent();
			EnemyWeapon->Destroy(); //Dödar vapnet 
			Die();
			//Destroy(); //Dödar fienden (Kommer ändras)
		}
		else
		{
			PlayEnemyHitReact();
		}
	}

	return DamageAmount;
}

//Daniel
void AEnemy::Die() const
{
	//stop behavoir tree and broadcast death
	AController* AIController = GetController();
	ensureMsgf(AIController != nullptr, TEXT("AI controller is nullptr"));
	if (AIController != nullptr)
	{
		UBehaviorTreeComponent* BehaviorComp = AIController->FindComponentByClass<UBehaviorTreeComponent>();
		ensureMsgf(BehaviorComp != nullptr, TEXT("BehavoirComp is nullptr"));
		if (BehaviorComp != nullptr)
		{
			BehaviorComp->StopTree();
		}
	}

	OnDeath.Broadcast();
}

//Daniel
void AEnemy::TargetLockPlayer(std::string teleport)
{
	bool bMoveLeft = FMath::RandBool(); // Randomize the movement direction

	//two behavoirs, decided by the method parameter string value

	//need collision check here
	if (teleport == "teleport")
	{
		UE_LOG(LogTemp, Warning, TEXT("bMoveLeft: %s"), bMoveLeft ? TEXT("True") : TEXT("False"));

		if (bMoveLeft)
		{
			// Move left
			FVector MoveOffset = FVector(0.0f, MoveDistanceFromPlayer, 0.0f);
			UE_LOG(LogTemp, Warning, TEXT("MoveOffset: %s"), *MoveOffset.ToString());
			AddActorLocalOffset(MoveOffset);
			ShouldTeleportEffectPlay = true;
		}
		else
		{
			// Move right
			FVector MoveOffset = FVector(0.0f, -MoveDistanceFromPlayer, 0.0f);
			UE_LOG(LogTemp, Warning, TEXT("MoveOffset: %s"), *MoveOffset.ToString());
			AddActorLocalOffset(MoveOffset);
			ShouldTeleportEffectPlay = true;
		}
	}
	else
	{
		AController* EnemyAIController = GetController();
		ensureMsgf(EnemyAIController != nullptr, TEXT("EnemyAIController is null."));

		if (EnemyAIController == nullptr) return;

		AAIController* AIController = Cast<AAIController>(EnemyAIController);
		ensureMsgf(AIController != nullptr, TEXT("AIController is null."));
		if (AIController == nullptr) return;

		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		ensureMsgf(Blackboard != nullptr, TEXT("Blackboard is null."));

		if (Blackboard == nullptr) return;

		FVector PlayerLocation = FVector::ZeroVector;
		FRotator PlayerRotation = FRotator::ZeroRotator;


		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		ensureMsgf(PlayerPawn != nullptr, TEXT("PlayerPawn is null."));

		// Get the player's location and rotation
		if (PlayerPawn)
		{
			PlayerLocation = PlayerPawn->GetActorLocation();
			PlayerRotation = PlayerPawn->GetActorRotation();
		}

		FVector MoveToLocation;

		//need collision check here
		if (bMoveLeft)
		{
			// Calculate left offset from player's location and rotation
			FVector LeftOffset = -(UKismetMathLibrary::GetRightVector(PlayerRotation) * MoveDistanceFromPlayer);
			MoveToLocation = PlayerLocation + LeftOffset;
		}
		else
		{
			// Calculate right offset from player's location and rotation
			FVector RightOffset = UKismetMathLibrary::GetRightVector(PlayerRotation) * MoveDistanceFromPlayer;
			MoveToLocation = PlayerLocation + RightOffset;
		}

		// Ensure that the MoveToLocation is always on the correct side of the player
		FVector DirectionToPlayer = PlayerLocation - GetActorLocation();
		FVector DirectionToMoveLocation = MoveToLocation - GetActorLocation();
		float DotProduct = FVector::DotProduct(DirectionToPlayer, DirectionToMoveLocation);

		if (DotProduct < 0)
		{
			MoveToLocation = PlayerLocation - DirectionToMoveLocation;
		}

		UE_LOG(LogTemp, Warning, TEXT("MoveToLocation: %s"), *MoveToLocation.ToString());
		Blackboard->SetValueAsVector("MoveAroundPlayerLocation", MoveToLocation);
	}
}

//Daniel
void AEnemy::MoveAlongTargetLock()
{
	if (!IsValid(PlayerTargetLock))
	{
		return;
	}

	AController* EnemyController = GetController();
	ensureMsgf(EnemyController != nullptr, TEXT("EnemyController is null."));
	if (EnemyController == nullptr) return;

	if (PlayerTargetLock)
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
		                                                              PlayerTargetLock->GetActorLocation());
		FRotator Offset = FRotator(-15.f, 0.f, 0.f);
		EnemyController->SetControlRotation(NewRotation + Offset);
	}
}

void AEnemy::SetTargetIndicator(bool Locked)
{
	UWidgetComponent* IndicatorWidgetComponent = Cast<UWidgetComponent>(
		GetComponentByClass(UWidgetComponent::StaticClass()));
	if (!IndicatorWidgetComponent) return;

	if (Locked)
	{
		IndicatorWidgetComponent->SetHiddenInGame(false);
	}
	else
	{
		IndicatorWidgetComponent->SetHiddenInGame(true);
	}
}

//Daniel
void AEnemy::ResetTargetLock()
{
	PlayerTargetLock = nullptr;
}

//Hugo
void AEnemy::PlaySound(USoundCue* Sound)
{
	if (AudioComponent && Sound)
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}
}
