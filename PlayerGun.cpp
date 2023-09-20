// Copyright Sabresaurus Ltd


#include "PlayerGun.h"


UPlayerGun::UPlayerGun()
{
}

void UPlayerGun::StartReload()
{
	if(GunProperties != nullptr)
	{
		const int RoundsAvailable = FGenericPlatformMath::Min(ReserveRounds, GunProperties->MagazineSize);
		RoundsInMagazine = RoundsAvailable;
		ReserveRounds -= RoundsAvailable;
	}
}

void UPlayerGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UPlayerGun::GetMuzzleLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	Cast<APawn>(GetOwner())->Controller->GetPlayerViewPoint(Location, Rotation);
}
