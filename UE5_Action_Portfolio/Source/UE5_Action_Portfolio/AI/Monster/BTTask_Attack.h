#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_MonsterBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Attack : public UBTTask_MonsterBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	bool AttackOrBlock = false;
	int MeleeAtackCount = 0;
	float BlockTime = 0;
};