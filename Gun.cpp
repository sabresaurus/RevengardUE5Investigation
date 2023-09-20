// Copyright Sabresaurus Ltd


#include "Gun.h"

#include "FPSTestbedProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGun::UGun()
{
}

// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();

	if(GunProperties != nullptr)
	{
		RoundsInMagazine = GunProperties->MagazineSize;
	}
}

void UGun::FireIndividualRound()
{
	if(GunProperties == nullptr)
	{
		return;
	}

	if(RoundsInMagazine < GunProperties->MinRoundCount)
	{
		// Not enough rounds
		return;
	}
	
	FVector MuzzleLoc;
	FRotator MuzzleRot;
	GetMuzzleLocationAndRotation(MuzzleLoc,MuzzleRot);

	AActor* Owner = GetOwner();

	APawn* Pawn = Cast<APawn>(Owner);

	float HitDistance;
	if(GunProperties->bIsHitScan)
	{
		const FVector TraceStart = MuzzleLoc;
		const FVector Direction = MuzzleRot.Vector();
		float MaxUseDistance = 10000;
		const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

		FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, Owner);
		// TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.bTraceComplex = true;

		/* FHitResults is passed in with the trace function and holds the result of the trace. */
		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);


		HitDistance = Hit.Distance;

		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			UGameplayStatics::ApplyDamage(HitActor, 10, Pawn->Controller, Owner, DamageType);

			USceneComponent* RootComponent = HitActor->GetRootComponent();
			
			if(RootComponent->IsSimulatingPhysics())
			{
				if(UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent); PrimitiveComponent != nullptr)
				{
					PrimitiveComponent->AddImpulseAtLocation(Direction*100000, Hit.Location);
				}
			}
		}
	}
	
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Pawn;

			// Spawn the projectile at the muzzle
			AFPSTestbedProjectile* Projectile = World->SpawnActor<AFPSTestbedProjectile>(ProjectileClass, MuzzleLoc + MuzzleRot.Vector() * 100, MuzzleRot, ActorSpawnParams);
			if(Projectile != nullptr)
			{
				Projectile->SetDamageType(DamageType);
				Projectile->SetIsHitscan(GunProperties->bIsHitScan);

				UProjectileMovementComponent* ProjectileMovement = Cast<UProjectileMovementComponent>(Projectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));
				if(ProjectileMovement != nullptr)
				{
					// TODO - Not sure this is working when testing it out
					Projectile->SetLifeSpan(HitDistance / ProjectileMovement->InitialSpeed);

					Projectile->SetLifeSpan(0.4);
				}
			}
		}
	}

	RoundsInMagazine--;
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetComponentLocation());
	}

	OnFireRound.Broadcast();
	//
	// // Try and play a firing animation if specified
	// if (FireAnimation != nullptr)
	// {
	// 	// Get the animation object for the arms mesh
	// 	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	// 	if (AnimInstance != nullptr)
	// 	{
	// 		AnimInstance->Montage_Play(FireAnimation, 1.f);
	// 	}
	// }
}

void UGun::GetMuzzleLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	Location = GetComponentLocation();
	Rotation = GetComponentRotation();
}

void UGun::FireStart()
{
	if(GunProperties == nullptr)
	{
		return;
	}
	
	const UWorld* World = GetWorld();
	if(GunProperties->bFullyAutomatic)
	{
		World->GetTimerManager().SetTimer(FireTimeHandle, this, &UGun::FireIndividualRound, 1.0f / GunProperties->RoundsPerSecond, true, 0);
	}
	else
	{
		// TODO: Stop this firing again too soon, should have a min time between rounds
		FireIndividualRound();	
	}
}

void UGun::FireStop()
{
	if(GunProperties == nullptr)
	{
		return;
	}
	
	if(GunProperties->bFullyAutomatic)
	{
		const UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(FireTimeHandle);
	}
}

void UGun::StartReload()
{
	if (GunProperties != nullptr)
	{
		RoundsInMagazine = GunProperties->MagazineSize;
	}
}

bool UGun::RequiresReload()
{
	if (GunProperties != nullptr)
	{
		return RoundsInMagazine >= GunProperties->MinRoundCount;
	}
	return true;
}
