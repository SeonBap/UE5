#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "Global/Enums.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:


protected:
	void NativeInitializeAnimation() override;
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTime) override;

public:
	MainCharacterAnimState Animstate = MainCharacterAnimState::Idle;
	TMap<MainCharacterAnimState, UAnimMontage*> AllAnimations;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Char, Meta = (AllowPrivateAccess = true))
	AMainCharacter* CurCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Char, Meta = (AllowPrivateAccess = true))
	UPawnMovementComponent* CurCharacterMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Char, Meta = (AllowPrivateAccess = true))
	int CurCharSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
};