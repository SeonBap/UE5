#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_MonsterBase.h"
#include "BTTask_WeaponMove.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_WeaponMove : public UBTTask_MonsterBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	int MoveTime = 0;
};