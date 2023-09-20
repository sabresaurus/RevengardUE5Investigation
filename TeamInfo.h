#pragma once

#include "TeamInfo.generated.h"

USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_BODY();
public:
	/* Display name for Team in-game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TeamName;

};