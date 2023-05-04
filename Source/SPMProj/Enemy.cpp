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
		AMeleeWeapon* EnemyWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponClass);
		EnemyWeapon->AttachWeaponOnPlayer(GetMesh(), FName("RightHandWeaponSocket"));
		EnemyWeapon->SetOwner(this);
		EnemyWeapon->SetInstigator(this);
		EnemyWeapon->GetComponentByClass(USphereComponent::StaticClass())->DestroyComponent();
		EquipedWeapon = EnemyWeapon;
	}
}

void AEnemy::EnemyAttackBasic()
{
	PlayEnemyAttackMontage();
}

UStatComponent* AEnemy::GetStats() const
{
	return Stats;
}

void AEnemy::SetWeaponCollison(ECollisionEnabled::Type Collision) const
{
	if(EquipedWeapon && EquipedWeapon->GetCollisionBox())
	{
		EquipedWeapon->GetCollisionBox()->SetCollisionEnabled(Collision);
		EquipedWeapon->ActorsToIgnore.Empty();
	}
}

void AEnemy::PlayEnemyAttackMontage() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && EnemyAttackMontage)
	{
		AnimInstance->Montage_Play(EnemyAttackMontage);
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
			if (ActorToSpawn)
			{
				GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation() - FVector(0,0,100), GetActorRotation());
			}
			
			Die();
			Destroy(); //Dödar fienden (Kommer ändras)
			EquipedWeapon->Destroy(); //Dödar vapnet 
		}
	}
	return DamageAmount;
}

void AEnemy::Die() const
{
	OnDeath.Broadcast();
}
