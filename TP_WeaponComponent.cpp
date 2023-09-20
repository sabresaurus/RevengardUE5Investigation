
#include "TP_WeaponComponent.h"

#include "EngineUtils.h"
#include "FPSTestbedCharacter.h"
#include "FPSTestbedProjectile.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Components/SpotLightComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::FireIndividualRound()
{
	if(Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if(bIsHitScan)
	{
		FVector CamLoc;
		FRotator CamRot;

		Character->Controller->GetPlayerViewPoint(CamLoc, CamRot);
		const FVector TraceStart = CamLoc;
		const FVector Direction = CamRot.Vector();
		float MaxUseDistance = 10000;
		const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

		FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, Character);
		// TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.bTraceComplex = true;

		/* FHitResults is passed in with the trace function and holds the result of the trace. */
		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			UGameplayStatics::ApplyDamage(HitActor, 10, Character->Controller, Character, DamageType);


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
	else
	{
		// Try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
				UE_LOG(LogTemp, Warning, TEXT("Fire2"));
			
				// Spawn the projectile at the muzzle
				AFPSTestbedProjectile* Projectile = World->SpawnActor<AFPSTestbedProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if(Projectile != nullptr)
				{
					Projectile->SetInstigator(Cast<APawn>(GetOwner()));
					Projectile->SetDamageType(DamageType);
				}
			}
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::FireStart()
{
	const UWorld* World = GetWorld();
	if(bFullyAutomatic)
	{
		World->GetTimerManager().SetTimer(FireTimeHandle, this, &UTP_WeaponComponent::FireIndividualRound, 0.1f, true, 0);
	}
	else
	{
		FireIndividualRound();	
	}
}

void UTP_WeaponComponent::FireStop()
{
	if(bFullyAutomatic)
	{
		const UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(FireTimeHandle);
	}
}

void UTP_WeaponComponent::SecondaryStart()
{
	UE_LOG(LogActor, Warning, TEXT("Test"));

	UActorComponent* SpotLightComponent = SpotLight.GetComponent(GetOwner());
	if(SpotLightComponent != nullptr)
	{
		static_cast<USceneComponent*>(SpotLightComponent)->ToggleVisibility();
	}
}

void UTP_WeaponComponent::SecondaryStop()
{
	
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister from the OnUseItem Event
		Character->OnUseItemStart.RemoveDynamic(this, &UTP_WeaponComponent::FireStart);

		Character->OnSecondaryStart.RemoveDynamic(this, &UTP_WeaponComponent::SecondaryStart);

	}
}

void UTP_WeaponComponent::AttachWeapon(AFPSTestbedCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		TArray<AActor*> Actors;
		Character->GetAttachedActors(Actors);
		for (int32 Idx = 0; Idx < Actors.Num(); Idx++)
		{
			UE_LOG( LogActor, Warning, TEXT("Destroying %s"), *Actors[Idx]->GetName() );
			Actors[Idx]->Destroy();
		}
		
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register so that Fire is called every time the character tries to use the item being held
		Character->OnUseItemStart.AddDynamic(this, &UTP_WeaponComponent::FireStart);
		Character->OnUseItemStop.AddDynamic(this, &UTP_WeaponComponent::FireStop);

		Character->OnSecondaryStart.AddDynamic(this, &UTP_WeaponComponent::SecondaryStart);
		Character->OnSecondaryStop.AddDynamic(this, &UTP_WeaponComponent::SecondaryStop);
	}
}

