#include "FPSTestbedGameMode.h"
#include "FPSTestbedCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSTestbedGameMode::AFPSTestbedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Revengard/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AFPSTestbedGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Bind our Player died delegate to the Gamemode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AFPSTestbedGameMode::PlayerDied);
	}
}

void AFPSTestbedGameMode::PlayerDied(ACharacter* Character)
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}
