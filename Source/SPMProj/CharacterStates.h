#pragma once

UENUM(BlueprintType) /*Kan användas i blueprints*/
enum class ECharacterWeaponState : uint8
{
	ECS_Unequiped UMETA(DisplayName = "Unequiped"),
	ECS_Equiped UMETA(DisplayName = "Equiped")
};
