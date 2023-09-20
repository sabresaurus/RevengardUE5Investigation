// Copyright Sabresaurus Ltd

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Components/ActorComponent.h"
#include "PlayerGun.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSTESTBED_API UPlayerGun : public UGun
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerGun();

	virtual void StartReload() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int ReserveRounds;

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetMuzzleLocationAndRotation(FVector& Location, FRotator& Rotation) const override;
};
