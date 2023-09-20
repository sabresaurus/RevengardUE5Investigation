// Copyright Sabresaurus Ltd

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class FPSTESTBED_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBase();

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	// Can't make GetActorEyesViewPoint BlueprintNative so redirect it to a function we can
	UFUNCTION(BlueprintNativeEvent)
	void GetActorEyesViewPointCustom(FVector& OutLocation, FRotator& OutRotation) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
