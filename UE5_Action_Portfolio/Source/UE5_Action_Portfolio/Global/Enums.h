#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	UnArmed,
	Bow,
	Sword,
	Shiled,
};


UENUM(BlueprintType)
enum class CharacterAnimState : uint8
{
	None,
	Idle,
	Walk,
	WalkJump,
	Run,
	RunJump,
	Roll,
	EquipOrDisArmBow,
	EquipOrDisArmSwordAndShield,
	LockOnIdle,
	LockOnBackward,
	LockOnForward,
	LockOnForwardRun,
	LockOnLeft,
	LockOnLeftRun,
	LockOnRight,
	LockOnRightRun,
	Attack,
	AimOrBlock,
	ParryorFire,
};

UENUM(BlueprintType)
enum class AIBTTask : uint8
{
	None UMETA(DisplayName = "Null"),
	Idle UMETA(DisplayName = "IDLE"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "CHASE"),
	Return UMETA(DisplayName = "RETURN"),
	Attack UMETA(DisplayName = "ATTACK")
};