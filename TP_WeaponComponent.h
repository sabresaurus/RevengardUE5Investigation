#pragma once

#include "CoreMinimal.h"
#include "TeamInfo.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class AFPSTestbedCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSTESTBED_API UTP_WeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSTestbedProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class UDamageType> DamageType;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool bIsHitScan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool bFullyAutomatic;

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "TeamName"))
	TArray<FTeamInfo> DefaultTeams;

	UPROPERTY(EditAnywhere, meta=(UseComponentPicker, AllowedClasses="ULightComponent"))
	FComponentReference SpotLight;
	FTimerHandle FireTimeHandle;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();


	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AFPSTestbedCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireStart();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireStop();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SecondaryStart();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SecondaryStop();

	UFUNCTION(CallInEditor)
	void TestFoo()
	{
		UE_LOG(LogTemp, Log, TEXT("TestFoo"));
	}

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void FireIndividualRound();

private:
	/** The Character holding this weapon*/
	AFPSTestbedCharacter* Character;
};
