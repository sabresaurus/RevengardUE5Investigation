#include "BaseAIController.h"

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(FGenericTeamId(8));
}

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if(Other.IsA(AFPSTestbedCharacter::StaticClass()))
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
