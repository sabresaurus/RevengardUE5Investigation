#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerGun.h"
#include "GameFramework/Character.h"
#include "FPSTestbedCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AFPSTestbedCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFPSTestbedCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UPlayerGun* PlayerGunSMG2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UPlayerGun* PlayerGunRifle2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UPlayerGun* PlayerGunRPG2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int ActiveGunIndex;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItemStart;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItemStop;
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnUseItem OnSecondaryStart;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnSecondaryStop;
protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UPlayerGun* GetGunByIndex(int WeaponIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	USkeletalMeshComponent* GetGunMeshByIndex(int WeaponIndex);
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	/** Fires a projectile. */
	void OnPrimaryActionPressed();
	void OnPrimaryActionReleased();

	void OnSecondaryActionPressed();
	void OnSecondaryActionReleased();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	AActor* GetUsableInView() const;

	UPlayerGun* ActiveGun;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	void ActivateGun(int WeaponIndex);
};

