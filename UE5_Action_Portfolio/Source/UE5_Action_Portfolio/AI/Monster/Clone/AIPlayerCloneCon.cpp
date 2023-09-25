#include "AI/Monster/Clone/AIPlayerCloneCon.h"
#include "AI/Monster/Clone/CloneMonster.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIPlayerCloneCon::AAIPlayerCloneCon()
{	
	GetAISenseConfigSight()->SightRadius = 2000.f;
	GetAISenseConfigSight()->LoseSightRadius = 3500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 90.f;
	GetAISenseConfigSight()->SetMaxAge(5.f);

	int Team = static_cast<int>(TeamType::CloneTeam);
	AAIController::SetGenericTeamId(FGenericTeamId(Team));
}

void AAIPlayerCloneCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAIPlayerCloneCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);
}

void AAIPlayerCloneCon::BeginPlay()
{
	Super::BeginPlay();
}
