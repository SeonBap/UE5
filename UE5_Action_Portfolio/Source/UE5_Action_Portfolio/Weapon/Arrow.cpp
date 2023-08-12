#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowScene = CreateDefaultSubobject<USceneComponent>(TEXT("Arrowcene"));
	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SetRootComponent(ArrowScene);

	ArrowSkeletalMesh->SetupAttachment(ArrowScene);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	ArrowSkeletalMesh->SetRelativeScale3D({ 2.f, 2.f, 1.5f });

	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 10000.f;

	InitialLifeSpan = 0.f;
}

void AArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 공격 충돌 처리
	//OtherActor->TakeDamage(10.f, );

	Destroy();
}

void AArrow::ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime)
{
	if (IsLocationAndRotation == true)
	{
		FVector SetPos = _Character->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
		
		SetActorLocation(SetPos);

		FVector Dir = _JointPos - SetPos;
		Dir.Normalize();

		SetActorRotation(Dir.Rotation());
	}
}

void AArrow::SetIsLocationAndRotation(bool _Value)
{
	IsLocationAndRotation = false;
}

void AArrow::ArrowChangeCollision(FName _FName)
{
	ArrowSkeletalMesh->SetCollisionProfileName(_FName, true);
}

void AArrow::FireInDirection(FVector _FVector, FRotator _FRotator)
{
	ProjectileMovement->SetUpdatedComponent(ArrowSkeletalMesh);
	ProjectileMovement->InitialSpeed = 10000.f;

	SetActorRotation(_FRotator);
	ProjectileMovement->Velocity = _FVector * ProjectileMovement->InitialSpeed;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	ArrowSkeletalMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Arrow")));

	ArrowSkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::ArrowBeginOverlap);
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

