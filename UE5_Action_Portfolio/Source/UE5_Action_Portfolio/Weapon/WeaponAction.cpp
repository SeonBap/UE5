// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global/GlobalCharAnimInstance.h"
#include "Global/GlobalCharacter.h"
#include "Weapon/BowAnimInstance.h"

UWeaponAction::UWeaponAction()
{	

}

void UWeaponAction::BeginPlay()
{
}

void UWeaponAction::Tick(float _DeltaTime)
{
	PressSpaceBarCkeckAndRoll(_DeltaTime);

	if (true == IsLockOn && (CharacterAnimState::Idle == AnimState || CharacterAnimState::Walk == AnimState))
	{
		AnimState = CharacterAnimState::LockOnIdle;
	}
	else if (false == IsLockOn && CharacterAnimState::LockOnIdle == AnimState)
	{
		AnimState = CharacterAnimState::Idle;
	}
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
}

void UWeaponAction::SetCharacterAirControl(float _Value)
{
	CurCharacter->GetCharacterMovement()->AirControl = _Value;
}

EWeaponType UWeaponAction::GetWeaponType()
{
	return WeaponType;
}

bool UWeaponAction::GetAttackCheck()
{
	return AttackCheck;
}

void UWeaponAction::SetLockOnCheck(bool _Value)
{
	IsLockOn = _Value;
}

bool UWeaponAction::GetLockOnCheck()
{
	return IsLockOn;
}

CharacterAnimState* UWeaponAction::GetAnimState()
{
	return &AnimState;
}

void UWeaponAction::SetAnimState(CharacterAnimState _State)
{
	AnimState = _State;
}

void UWeaponAction::IsRollMoveToFalse()
{
	IsRollMove = false;
}

void UWeaponAction::ChangeWeapon(FName _Weapon)
{
	UGlobalCharAnimInstance* Ptr = Cast<UGlobalCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	UGlobalGameInstance* Instacne = CurCharacter->GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const struct FAnimaitionData* FindAnimationData = Instacne->GetAllAnimaitionDatas(_Weapon);

	if (nullptr == FindAnimationData)
	{
		return;
	}

	Ptr->AllAnimations = FindAnimationData->AllAnimations;
	WeaponType = FindAnimationData->Type;
}

void UWeaponAction::ChangeSetUnArmed()
{
	// Idle과 LockOnIdle만 무기 변경 가능, 장비를 끼고 있어야 UnArmed 가능
	if ((CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState) || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// 역재생을 위한 AnimInstance
	UGlobalCharAnimInstance* Ptr = Cast<UGlobalCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	// 착용하고 있던 장비가 활일 때
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmBow;
	}
	// 착용하고 있던 장비가 칼일 때
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	// Idle과 LockOnIdle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
	{
		return;
	}
	// 같은 무기로 변경시 장비 장착 해제
	else if (EWeaponType::Bow == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// 다른 무기로 변경시 해체하고 장비
	else if (EWeaponType::Sword == WeaponType)
	{
		// 장비 변경 애니메이션이 안됨
		ChangeSetUnArmed();
		SwordAndSheiledToBow = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmBow;

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	// Idle과 LockOnIdle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
	{
		return;
	}
	//같은 무기로 변경시 장비 장착 해제
	else if (EWeaponType::Sword == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// 다른 무기로 변경시 해체하고 장비
	else if (EWeaponType::Bow == WeaponType)
	{
		// 장비 변경 애니메이션이 안됨
		ChangeSetUnArmed();
		BowToSwordAndSheiled = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;

	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	// SpaceBar 누른 시간 체크
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	if (true == IsRollMove)
	{
		// 이렇게 if문 4개가 아닌 FVector로 할 수 있는가???
		// 방향 돌리기
		if (true == IsLockOn)
		{
			FRotator Rotation;
			FVector Angle;

			if (true == IsForwardWalk)
			{
				Angle = CurCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsBackwardWalk)
			{
				Angle = -CurCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsLeftWalk)
			{
				Angle += -CurCharacter->GetActorRightVector();
				Angle.Z = 0;
			}
			if (true == IsRightWalk)
			{
				Angle += CurCharacter->GetActorRightVector();
				Angle.Z = 0;
			}

			Rotation = Angle.Rotation();
			CurCharacter->SetActorRotation(Rotation);
		}

		// 움직이기
		FVector DeltaLocation = CurCharacter->GetActorRotation().Vector();

		DeltaLocation.X = 600 * _DeltaTime;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
	}
}

bool UWeaponAction::LockOnAfterRun(float _DeltaTime)
{
	if (true == LockOnCheck)
	{
		LockOnAfterRunTime += _DeltaTime;
	}

	if (LockOnAfterRunCount < LockOnAfterRunTime)
	{
		LockOnCheck = false;
		LockOnAfterRunTime = 0.f;

		return true;
	}

	return false;
}

void UWeaponAction::WAndSButtonAction(float _Value)
{
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
	MoveXValue = 0;
		return;
		break;
	}

	MoveXValue = _Value;

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			AnimState = CharacterAnimState::Walk;
		case CharacterAnimState::Walk:
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case CharacterAnimState::Run:
			break;
		case CharacterAnimState::LockOnIdle:
		case CharacterAnimState::LockOnBackward:
		case CharacterAnimState::LockOnForward:
			if (-1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnBackward;
			}
			else if (1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnForward;
			}
			LockOnAfterRunTime = 0.f;
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
			break;
		case CharacterAnimState::LockOnForwardRun:
			break;
		}


		if (-1.f == _Value)
		{
			IsForwardWalk = false;
			IsBackwardWalk = true;
		}
		else if (1.f == _Value)
		{
			IsForwardWalk = true;
			IsBackwardWalk = false;
		}

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		CurCharacter->AddMovementInput(Direction, _Value);

	}
	else
	{
		// 앞뒤 입력이 없으면 false
		if (0.f == _Value)
		{
			IsForwardWalk = false;
			IsBackwardWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnForward == AnimState || CharacterAnimState::LockOnBackward == AnimState) && (false == IsLeftWalk && false == IsRightWalk))
		{
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
			else if (true == IsLockOn)
			{
				AnimState = CharacterAnimState::LockOnIdle;
			}
		}
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
	MoveYValue = 0;
		return;
		break;
	}

	MoveYValue = _Value;

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			AnimState = CharacterAnimState::Walk;
		case CharacterAnimState::Walk:
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case CharacterAnimState::Run:
			break;
		case CharacterAnimState::LockOnIdle:
		case CharacterAnimState::LockOnLeft:
		case CharacterAnimState::LockOnRight:
			if (-1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnLeft;
			}
			else if (1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnRight;
			}
			LockOnAfterRunTime = 0.f;
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
			break;
		case CharacterAnimState::LockOnForwardRun:
			break;
		}

		if (-1.f == _Value)
		{
			IsLeftWalk = true;
			IsRightWalk = false;
		}
		else if (1.f == _Value)
		{
			IsLeftWalk = false;
			IsRightWalk = true;
		}

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		// 좌우 입력이 없으면 false
		if (0.f == _Value)
		{
			IsLeftWalk = false;
			IsRightWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnLeft == AnimState || CharacterAnimState::LockOnRight == AnimState) && (false == IsForwardWalk && false == IsBackwardWalk))
		{
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
			else if (true == IsLockOn)
			{
				AnimState = CharacterAnimState::LockOnIdle;
			}
		}
	}
}

void UWeaponAction::RollorRunAction(float _Value)
{
	if (0.f == _Value)
	{
		if (AnimState == CharacterAnimState::Run)
		{
			if (true == IsLockOn)
			{
				LockOnCheck = true;
				AnimState = CharacterAnimState::LockOnIdle;
			}
			else if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
		}

		// 짧게 입력이 들어왔는지 확인
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// 구르면 안되는 상태
			switch (AnimState)
			{
			case CharacterAnimState::WalkJump:
			case CharacterAnimState::RunJump:
			case CharacterAnimState::Roll:
			case CharacterAnimState::EquipOrDisArmBow:
			case CharacterAnimState::EquipOrDisArmSwordAndShield:
			case CharacterAnimState::Attack:
			case CharacterAnimState::ParryorFire:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			
			// 구른다
			IsRollMove = true;
			AnimState = CharacterAnimState::Roll;
		}

		// 입력이 멈추면 누른 시간 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// 달리기와 구르면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	// 점프가 끝나고도 달리기 유지
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
		return;
		break;
	}

	// 누른 시간 체크 시작
	if (false == PressSpacebar)
	{
		PressSpacebar = true;
	}

	if (false == IsForwardWalk && false == IsBackwardWalk && false == IsLeftWalk && false == IsRightWalk)
	{
		PressSpacebarTime = 0;
		PressSpacebar = false;

		if (true == IsLockOn)
		{
			LockOnCheck = true;
			AnimState = CharacterAnimState::LockOnIdle;
		}
		else if (false == IsLockOn)
		{
			AnimState = CharacterAnimState::Idle;
		}

		return;
	}

	// 달리면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::AimOrBlock:
	case CharacterAnimState::LockOnIdle:
		return;
	}

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// 달린다
		if (true == IsLockOn)
		{
			LockOnAfterRunTime = 0.f;
			CurCharacter->bUseControllerRotationYaw = false;
		}

		AnimState = CharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// 점프하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	// 달릴 때 점프
	if (CharacterAnimState::Run == AnimState)
	{
		AnimState = CharacterAnimState::RunJump;
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		CurCharacter->Jump();
	}
	// 걸을 때, 가만히 있을 때 점프
	else
	{
		AnimState = CharacterAnimState::WalkJump;
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}

void UWeaponAction::AttackAction()
{
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		AnimState = CharacterAnimState::Attack;
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// 조준 중이며 화살이 준비 된 상태
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			AnimState = CharacterAnimState::ParryorFire;
		}
		// 조준 중이며 화살이 준비 안된 상태
		else if (CharacterAnimState::AimOrBlock == AnimState && false == EarlyArrowCheck)
		{
			EarlyArrowCheck = true;
		}
		// 조준이 아닌 일반 공격인 상태
		else if (CharacterAnimState::AimOrBlock != AnimState && false == ArrowReady)
		{
			AnimState = CharacterAnimState::Attack;
		}
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState)
		{
			AttackCheck = true;
		}

		AnimState = CharacterAnimState::Attack;
	}
}

void UWeaponAction::AimorBlockAtion(float _Value)
{
	if (0 == _Value)
	{
		if (CharacterAnimState::AimOrBlock == AnimState)
		{
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Idle;
		}

		if (EWeaponType::Bow == WeaponType && CharacterAnimState::Attack == AnimState)
		{
			return;
		}

		AGlobalCharacter* character = Cast<AGlobalCharacter>(CurCharacter);

		if (nullptr == character && false == character->IsValidLowLevel())
		{
			return;
		}

		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(character->BowWeaponMesh->GetAnimInstance());

		if (nullptr == BowAnim)
		{
			return;
		}


		BowAnim->SetBowChordCheck(false);

		return;
	}

	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Run:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		return;
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		AnimState = CharacterAnimState::AimOrBlock;
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = AimorBlockSpeed;
		AnimState = CharacterAnimState::AimOrBlock;
	}
}

float UWeaponAction::GetMoveXValue()
{
	return MoveXValue;
}

float UWeaponAction::GetMoveYValue()
{
	return MoveYValue;
}

bool UWeaponAction::GetIsLockOn()
{
	return IsLockOn;
}
