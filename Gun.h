// Copyright Sabresaurus Ltd

#pragma once

#include "CoreMinimal.h"
#include "GunProperties.h"
#include "Components/ActorComponent.h"
#include "Gun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunFireRound);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSTESTBED_API UGun : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FireIndividualRound();

	virtual void GetMuzzleLocationAndRotation(FVector& Location, FRotator& Rotation) const;

	FTimerHandle FireTimeHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int RoundsInMagazine = 0;

	UPROPERTY(BlueprintAssignable)
	FGunFireRound OnFireRound;
	

public:
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSTestbedProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	UGunProperties* GunProperties;
	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;


	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	UAnimMontage* FireAnimation;

	UFUNCTION(BlueprintCallable)
	void FireStart();

	UFUNCTION(BlueprintCallable)
	void FireStop();

	UFUNCTION(BlueprintCallable)
	virtual void StartReload();

	UFUNCTION(BlueprintCallable)
	bool RequiresReload();
	
};
