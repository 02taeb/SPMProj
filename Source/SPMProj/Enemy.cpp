// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "MeleeWeapon.h"
#include "StatComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Components dont need to be attached.
	Stats = CreateDefaultSubobject<UStatComponent>("Stats");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	/*Tag, mostly to check weapon collision*/
	Tags.Add(FName("Enemy"));
	
	if(GetWorld() && WeaponClass)
	{
		EnemyWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponClass);
		EnemyWeapon->AttachWeaponOnPlayer(GetMesh(), FName("RightHandWeaponSocket"));
		EnemyWeapon->SetOwner(this);
		EnemyWeapon->SetInstigator(this);
		EnemyWeapon->GetComponentByClass(USphereComponent::StaticClass())->DestroyComponent();
		//EquipedWeapon = EnemyWeapon;
	}
}

void AEnemy::EnemyAttackBasic()
{
	PlayEnemyAttackMontage();
}

void AEnemy::PlayEnemyHitReact()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && EnemyHitReactMontage)
	{
		AnimInstance->Montage_Play(EnemyHitReactMontage);
		
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
		
		AnimInstance->Montage_JumpToSection(SectionToPlay, EnemyHitReactMontage);
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
	if(CrossProduct.Z < 0) HitAngle *= -1.f;
}


UStatComponent* AEnemy::GetStats() const
{
	return Stats;
}

void AEnemy::SetWeaponCollison(ECollisionEnabled::Type Collision) const
{
	if(EnemyWeapon && EnemyWeapon->GetCollisionBox())
	{
		EnemyWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
		EnemyWeapon->ActorsToIgnore.Empty();
	}
}

void AEnemy::PlayEnemyAttackMontage() 
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && EnemyAttackMontage)
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
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if(Stats)
	{	
		
		Stats->TakeDamage(DamageAmount);		
		if(Stats->Dead())
		{
			/*
			if (ActorToSpawn)
			{
				GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation() - FVector(0,0,100), GetActorRotation());
			}
			*/
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->StopAllMontages(0.1f);
			}
			GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
			OnDeathBPEvent();
			EnemyWeapon->Destroy(); //Dödar vapnet 
			Die();
			//Destroy(); //Dödar fienden (Kommer ändras)
		} else
		{
			PlayEnemyHitReact();
		}
	}
	
	return DamageAmount;
}

void AEnemy::Die() const
{
	OnDeath.Broadcast();
}
