#pragma once
/*Enum for relationship between the character and the weapon*/
UENUM(BlueprintType) /*Kan användas i blueprints*/
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequiped UMETA(DisplayName = "Unequiped"),
	ECWS_Equiped UMETA(DisplayName = "Equiped")
};
/*Enum for all action states, like attacking, drinking a health potion, eating an enemy and so on...*/
enum class ECharacterActionState : uint8
{
	ECAS_NoAction UMETA(DisplayName = "Unequiped"),
	ECAS_Attacking UMETA(DisplayName = "Equiped")
};
