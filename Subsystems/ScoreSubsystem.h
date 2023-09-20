// Copyright Sabresaurus Ltd

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ScoreSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FPSTESTBED_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ValorCollected;

	UFUNCTION(BlueprintCallable)
	void AddValor(int Delta);

	UFUNCTION(Exec)
	void SetValor(int NewValor);
};
