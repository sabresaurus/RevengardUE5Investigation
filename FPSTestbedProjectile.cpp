#include "FPSTestbedProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

void AFPSTestbedProjectile::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UE_LOG(LogActor, Log, TEXT("Create"));
}

AFPSTestbedProjectile::AFPSTestbedProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSTestbedProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFPSTestbedProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIsHitScan)
	{
		// Do nothing if hitscan, we're just cosmetic
		return;
	}
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *OtherActor->GetName());
		//
		// UGeometryCollectionComponent a;
		// a.NotifyBreak()

		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageType);
		//UGameplayStatics::ApplyPointDamage(OtherActor, 10, Hit.Normal, Hit, GetInstigatorController(), this, DamageType);
	}
	
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 1.0f, GetActorLocation());
	}

	Destroy();
}

void AFPSTestbedProjectile::SetDamageType(const TSubclassOf<UDamageType>& NewDamageType)
{
	DamageType = NewDamageType;
}

void AFPSTestbedProjectile::SetIsHitscan(bool bNewIsHitScan)
{
	bIsHitScan = bNewIsHitScan;
}
