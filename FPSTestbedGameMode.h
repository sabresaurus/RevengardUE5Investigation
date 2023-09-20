#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSTestbedGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS(minimalapi)
class AFPSTestbedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }
	
	AFPSTestbedGameMode();


protected:
	virtual void BeginPlay() override;
	
	//Called when Player character has died.
	UFUNCTION(BlueprintCallable)
	virtual void PlayerDied(ACharacter* Character);

	//Signature to bind delegate. 
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
};



