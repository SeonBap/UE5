#include "AI/Monster/Clone/CloneMonster.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "Weapon/WeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Character/MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ACloneMonster::ACloneMonster()
{
	SetAnimState<CharacterAnimState>(CharacterAnimState::Idle);

	SetActorTypeTag(TEXT("Clone"));
	SetAttackTypeTag(TEXT("CloneAttack"));
	Tags.Add(GetActorTypeTag());

	// 락온을 위해 추가하는 태그
	Tags.Add(TEXT("Monster"));

	SetParrybool(true);

	DataName = "CloneMonster";

	float CapsuleSize = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	CurCapsuleSize = CapsuleSize;
}

void ACloneMonster::AIInit()
{
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	const FMonsterData* MonsterData = Instance->GetMonsterData(DataName);

	if (nullptr == MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == MonsterData"), __FUNCTION__, __LINE__);
		return;
	}

	SetHP(MonsterData->HP);
	SetMaxHP(GetHP());

	UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

	if (false == IsValid(CurBlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBlackboardComponent->SetValueAsObject(TEXT("SelfActor"), this);
	CurBlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	CurBlackboardComponent->SetValueAsObject(TEXT("NavPath"), nullptr);
	CurBlackboardComponent->SetValueAsFloat(TEXT("StateTime"), 0.f);
	CurBlackboardComponent->SetValueAsFloat(TEXT("PatrolRange"), 800.f);
	CurBlackboardComponent->SetValueAsFloat(TEXT("MeleeAttackRange"), 180.f);
	CurBlackboardComponent->SetValueAsInt(TEXT("PatrolCount"), 0);
	CurBlackboardComponent->SetValueAsInt(TEXT("NavPathIndex"), 1);
	CurBlackboardComponent->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	CurBlackboardComponent->SetValueAsVector(TEXT("PatrolPos"), GetActorLocation());
	CurBlackboardComponent->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsReturn"), false);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsDeath"), false);
}

void ACloneMonster::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;
}

void ACloneMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

float ACloneMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (0.f >= GetHP())
	{
		APawn* EventInstigatorPawn = EventInstigator->GetPawn();

		if (false == IsValid(EventInstigatorPawn))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

		if (false == IsValid(CurWeaponComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

		if (false == IsValid(CurBlackboardComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		UCapsuleComponent* CurCapsuleComponent = GetCapsuleComponent();

		if (false == IsValid(CurCapsuleComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		USkeletalMeshComponent* CurSkeletalMeshComponent = GetMesh();

		if (false == IsValid(CurCapsuleComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		// 죽음
		CurBlackboardComponent->SetValueAsBool(TEXT("IsDeath"), true);
		CurCapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"), true);
		CurSkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"), true);
		CurWeaponComponent->Death();

		bool PlayerCheck = EventInstigatorPawn->ActorHasTag("Player");

		if (true == PlayerCheck)
		{
			AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigatorPawn);

			if (false == IsValid(MainChar))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}

			MainChar->LostLockedOnTargetActor();
		}
		
		return FinalDamage;
	}

	return FinalDamage;
}