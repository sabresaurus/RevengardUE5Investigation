#pragma once
#include "FPSTestbedCharacter.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "BaseAIController.generated.h"

// UCLASS(ClassGroup = AI, BlueprintType, Blueprintable)
UCLASS(config=Game)
class ABaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
public:
	
};
