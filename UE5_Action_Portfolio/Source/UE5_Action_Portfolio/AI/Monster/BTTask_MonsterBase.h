#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BTTask_AIBase.h"
#include "Global/GlobalAICharacter.h"
#include "Global/GlobalGameInstance.h"
#include "BTTask_MonsterBase.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_MonsterBase : public UBTTask_AIBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MonsterBase();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnGameplayTaskActivated(class UGameplayTask&) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
