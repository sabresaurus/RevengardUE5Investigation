#include "FPSTestbedCharacter.h"
#include "FPSTestbedProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// AFPSTestbedCharacter

AFPSTestbedCharacter::AFPSTestbedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//RootComponent
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AFPSTestbedCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AFPSTestbedCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GetUsableInView();
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSTestbedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSTestbedCharacter::OnPrimaryActionPressed);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this,
	                                 &AFPSTestbedCharacter::OnPrimaryActionReleased);

	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this,
	                                 &AFPSTestbedCharacter::OnSecondaryActionPressed);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this,
	                                 &AFPSTestbedCharacter::OnSecondaryActionReleased);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSTestbedCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSTestbedCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFPSTestbedCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFPSTestbedCharacter::LookUpAtRate);
}

FGenericTeamId AFPSTestbedCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(10);
}

void AFPSTestbedCharacter::OnPrimaryActionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPrimaryActionPressed"))
	OnUseItemStart.Broadcast();
}

void AFPSTestbedCharacter::OnPrimaryActionReleased()
{
	OnUseItemStop.Broadcast();
}

void AFPSTestbedCharacter::OnSecondaryActionPressed()
{
	OnSecondaryStart.Broadcast();
}

void AFPSTestbedCharacter::OnSecondaryActionReleased()
{
	OnSecondaryStop.Broadcast();
}

void AFPSTestbedCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSTestbedCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSTestbedCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSTestbedCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


/*
	Performs ray-trace to find closest looked-at UsableActor.
*/

AActor* AFPSTestbedCharacter::GetUsableInView() const
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return nullptr;

	/* This retrieves are camera point of view to find the start and direction we will trace. */
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	float MaxUseDistance = 10000;
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	// TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	/* FHitResults is passed in with the trace function and holds the result of the trace. */
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	// if(Hit.GetActor() != nullptr)
	// {
	// 	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 1.0f);
	// 	DrawDebugLine(GetWorld(), Hit.GetActor()->GetActorLocation(), TraceEnd, FColor::Blue, false, 1.0f);
	// }
	// else
	// {
	// 	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);
	// }

	return Hit.GetActor();
	// return Cast<ASUsableActor>(Hit.GetActor());
}

void AFPSTestbedCharacter::ActivateGun(int WeaponIndex)
{
	if (ActiveGun != nullptr)
	{
		OnUseItemStart.RemoveDynamic(ActiveGun, &UPlayerGun::FireStart);
		OnUseItemStop.RemoveDynamic(ActiveGun, &UPlayerGun::FireStop);
	}

	for (int I = 0; I<3; I++)
	{
		USkeletalMeshComponent* GunMesh = GetGunMeshByIndex(I);
		GunMesh->SetVisibility(WeaponIndex == I);
	}

	UPlayerGun* Gun = GetGunByIndex(WeaponIndex);
	if (Gun != nullptr)
	{
		ActiveGunIndex = WeaponIndex;
		ActiveGun = Gun;
		OnUseItemStart.AddDynamic(ActiveGun, &UPlayerGun::FireStart);
		OnUseItemStop.AddDynamic(ActiveGun, &UPlayerGun::FireStop);
	}
}
