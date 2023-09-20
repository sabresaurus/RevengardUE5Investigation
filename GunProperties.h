// Copyright Sabresaurus Ltd

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GunProperties.generated.h"

/**
 * 
 */
UCLASS()
class FPSTESTBED_API UGunProperties : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool bIsHitScan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool bFullyAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int RoundsPerSecond = 3;

	/** Some guns have a burst fire and must fire X rounds in a row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int MinRoundCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int MagazineSize = 12;
};
