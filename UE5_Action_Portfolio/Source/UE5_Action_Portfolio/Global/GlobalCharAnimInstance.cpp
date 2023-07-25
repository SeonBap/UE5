#include "Global/GlobalCharAnimInstance.h"
#include "Global/GlobalCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Global/AnimaitionData.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponAction.h"

void UGlobalCharAnimInstance::MontageEnd(UAnimMontage* Anim, bool Inter)
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (AllAnimations[CharacterAnimState::WalkJump] == Anim || AllAnimations[CharacterAnimState::RunJump] == Anim || 
		AllAnimations[CharacterAnimState::Roll] == Anim || AllAnimations[CharacterAnimState::Attack] == Anim)
	{
		Animstate = CharacterAnimState::Idle;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.f);
		character->CurWeaponAction->SetCharacterAirControl(1.f);
	}
	else if (AllAnimations[CharacterAnimState::EquipOrDisArmBow] == Anim || AllAnimations[CharacterAnimState::EquipOrDisArmSwordAndShield] == Anim)
	{
		Animstate = CharacterAnimState::Idle;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.f);
		AnimSpeed = 1.f;

		if (true == character->CurWeaponAction->SwordAndSheiledToBow)
		{
			character->CurWeaponAction->ChangeSetBow();
			character->CurWeaponAction->SwordAndSheiledToBow = false;
		}
		else if (true == character->CurWeaponAction->BowToSwordAndSheiled)
		{
			character->CurWeaponAction->ChangeSetSwordAndSheiled();
			character->CurWeaponAction->BowToSwordAndSheiled = false;
		}
	}
	else if (AllAnimations[CharacterAnimState::ParryorFire] == Anim)
	{
		if (EWeaponType::Bow == character->CurWeaponAction->WeaponType)
		{
			Animstate = CharacterAnimState::AimOrBlock;
			character->CurWeaponAction->SetAnimState(Animstate);
			Montage_Play(AllAnimations[CharacterAnimState::AimOrBlock], 1.f);
			character->CurWeaponAction->ArrowReady = false;
			character->CurWeaponAction->EarlyArrowCheck = false;
		}
	}
}

void UGlobalCharAnimInstance::AnimNotify_RollStop()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->CurWeaponAction->IsRollMoveToFalse();
}

void UGlobalCharAnimInstance::AnimNotify_JumpStart()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UGlobalCharAnimInstance::AnimNotify_ChangeWeapon()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}
	
	EWeaponType CurWeaponType = character->CurWeaponAction->WeaponType;
	CharacterAnimState CurAnimState = character->CurWeaponAction->AnimState;

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	// 전환 하려는 무기가 맨손일 때
	if (EWeaponType::UnArmed == CurWeaponType)
	{
		// 활을 들고 있을 때
		if (CharacterAnimState::EquipOrDisArmBow == CurAnimState)
		{
			character->BowWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}
		// 칼과 방패를 들고 있을 때
		else if (CharacterAnimState::EquipOrDisArmSwordAndShield == CurAnimState)
		{
			character->SwordWeaponMesh->SetSkeletalMesh(nullptr);
			character->ShieldWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
			character->BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}

	}
	// 전환 하려는 무기가 활일 때
	else if (EWeaponType::Bow == CurWeaponType)
	{
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackBowWeaponMesh->SetSkeletalMesh(nullptr);
		character->BowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));

	}
	// 전환 하려는 무기가 칼과 방패일 때
	else if (EWeaponType::Sword == CurWeaponType)
	{
		character->SwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
		character->ShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackSwordWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackShieldWeaponMesh->SetSkeletalMesh(nullptr);
	}
}

void UGlobalCharAnimInstance::AnimNotify_AttackCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	bool Value = character->CurWeaponAction->AttackCheck;

	if (CharacterAnimState::Attack == character->CurWeaponAction->AnimState)
	{
		if (false == Value)
		{
			Animstate = CharacterAnimState::Idle;
			character->CurWeaponAction->SetAnimState(Animstate);
			Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.0f);
		}
		else
		{
			character->CurWeaponAction->AttackCheck = false;
		}
	}
}

void UGlobalCharAnimInstance::AnimNotify_AimorBlockCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (CharacterAnimState::AimOrBlock == character->CurWeaponAction->AnimState)
	{
		// 다시 에임or 블락
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[Animstate];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UGlobalCharAnimInstance::AnimNotify_ArrowReadyCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	// 미리 공격 입력을 눌렀을 때
	if (true == character->CurWeaponAction->EarlyArrowCheck && CharacterAnimState::AimOrBlock == Animstate)
	{
		character->CurWeaponAction->ArrowReady = false;
		character->CurWeaponAction->EarlyArrowCheck = false;

		Animstate = CharacterAnimState::ParryorFire;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::ParryorFire], 1.f);
	}
	// 준비된 후 입력을 받으려 할 때
	else if (false == character->CurWeaponAction->EarlyArrowCheck)
	{
		character->CurWeaponAction->ArrowReady = true;
	}
}

void UGlobalCharAnimInstance::NativeInitializeAnimation()
{
}

void UGlobalCharAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UGlobalCharAnimInstance::MontageEnd);

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	struct FAnimaitionData* FindAnimaitionData = Instance->GetAllAnimaitionDatas(TEXT("UnArmed"));

	if (nullptr == FindAnimaitionData)
	{
		return;
	}

	AllAnimations = FindAnimaitionData->AllAnimations;
}

void UGlobalCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	Animstate = *character->AnimState;

	if (false == AllAnimations.Contains(Animstate))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[Animstate];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		// 무기가 있는 상태에서 없는 상태로 갈 땐 역재생
		if (EWeaponType::UnArmed == character->CurWeaponAction->WeaponType && (CharacterAnimState::EquipOrDisArmBow == Animstate || CharacterAnimState::EquipOrDisArmSwordAndShield == Animstate))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// 나머지 일반적인 애니메이션 재생
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(Montage, AnimSpeed);
		}
	}
}
