#include "AI/Monster/BTTask_Clone_Equip.h"

EBTNodeResult::Type UBTTask_Clone_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (nullptr == CloneMonster || false == CloneMonster->IsValidLowLevel())
	{
		return EBTNodeResult::Failed;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return EBTNodeResult::Failed;
	}

	CloneMonster->bUseControllerRotationYaw = true;

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = CloneMonster->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	WeaponComponent->SetIsLockOn(true);

	if (800 <= Dir.Size())
	{
		if (EWeaponType::Bow != CurWeaponType)
		{
			WeaponComponent->ChangeSetBow();
		}
	}
	else
	{
		if (EWeaponType::Sword != CurWeaponType)
		{
			WeaponComponent->ChangeSetSwordAndSheiled();
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (nullptr == CloneMonster || false == CloneMonster->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = CloneMonster->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	if (600 <= Dir.Size())
	{
		if (EWeaponType::Bow == CurWeaponType)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if (EWeaponType::Sword == CurWeaponType)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	if (EWeaponType::UnArmed == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
