#pragma once
/*Enum for relationship between the character and the weapon*/
UENUM(BlueprintType) /*Kan användas i blueprints*/
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequiped UMETA(DisplayName = "Unequiped"),
	ECWS_Equiped UMETA(DisplayName = "Equiped")
};
/*Enum for all action states, like attacking, drinking a health potion, eating an enemy and so on...*/
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_NoAction UMETA(DisplayName = "NoAction"),
	ECAS_Attacking UMETA(DisplayName = "Attacking")
};
