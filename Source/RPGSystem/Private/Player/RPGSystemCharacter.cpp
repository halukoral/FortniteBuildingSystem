// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RPGSystemCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// ARPGSystemCharacter

ARPGSystemCharacter::ARPGSystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//////////////////////////////////////////////////////////////////////////
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		CombatCrossHairTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Crosshairs/Crosshair_Combat.Crosshair_Combat'"));
	if (CombatCrossHairTexture.Object != nullptr)
		CrossHairCom = CombatCrossHairTexture.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		ConstructionCrossHairTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Crosshairs/Crosshair_Construction.Crosshair_Construction'"));
	if (ConstructionCrossHairTexture.Object != nullptr)
		CrossHairConst = ConstructionCrossHairTexture.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		CombatTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/States/Icon_Combat.Icon_Combat'"));
	if (CombatTexture.Object != nullptr)
		IconCombat = CombatTexture.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		ConstructionTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/States/Icon_Construction.Icon_Construction'"));
	if (ConstructionTexture.Object != nullptr)
		IconConstruct = ConstructionTexture.Object;
}


void ARPGSystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ARPGSystemCharacter::SightTrace, 0.05f, true);
}

void ARPGSystemCharacter::ChangeState(EPlayerStates NewState)
{
	if (NewState != PlayerStates)
	{
		PlayerStates = NewState;
		UpdateStateDisplay();
	}
}

void ARPGSystemCharacter::UpdateStateDisplay()
{
	if ( UUtils::Test(5, MainWidget, BuildingManager, ResourceManager, (__FUNCTION__), __LINE__) == false) return;
	
	if (PlayerStates == EPlayerStates::VE_Combat)
	{
		MainWidget->GetCrossHair()->SetBrushFromTexture(CrossHairCom, true);
		MainWidget->GetStateIcon()->SetBrushFromTexture(IconConstruct, true);
		MainWidget->GetBuildingBox()->SetRenderOpacity(0.8f);
		MainWidget->GetBuildingBox()->SetRenderScale(FVector2D(0.8f,0.8f));
		BuildingManager->OnStopBuilding();
		MainWidget->ShowCurrentResource(false);
	}
	else if (PlayerStates == EPlayerStates::VE_Construction)
	{
		MainWidget->GetCrossHair()->SetBrushFromTexture(CrossHairConst, true);
		MainWidget->GetStateIcon()->SetBrushFromTexture(IconCombat, true);
		MainWidget->GetBuildingBox()->SetRenderOpacity(1.f);
		MainWidget->GetBuildingBox()->SetRenderScale(FVector2D(1.f, 1.f));
		BuildingManager->SelectCurrentBuilding(true);
		MainWidget->UpdateCurrentResource(ResourceManager);
		MainWidget->ShowCurrentResource(true);
	}
}

void ARPGSystemCharacter::SightTrace()
{
	if (UUtils::Test(3, BuildingManager, (__FUNCTION__), __LINE__) == false) return;

	FVector Location = BuildingManager->GetPlayerLookAtLocation(BuildingManager->GetForwardBuildingOffset() * 3);

	FVector Start =	GetActorLocation();
	FVector End = BuildingManager->ToGridLocation(Location);
	
	FTwoVectors TracePoints = GetLineTracePoints();

	FHitResult Hit;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), false, this);
	//DrawDebugLine(GetWorld(), TracePoints.v1, TracePoints.v2, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	/// Line-trace (AKA ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		TracePoints.v1,
		TracePoints.v2,
		ECC_Visibility,
		TraceParams
	);
	
	ABuilding* Building = Cast<ABuilding>(Hit.GetActor());

	if (IsValid(Building))
	{
		if (IsValid(CurrentlySeenBuilding) && Building != CurrentlySeenBuilding)
		{
			CurrentlySeenBuilding->OnLeavePlayerSight();
			CurrentlySeenBuilding = Building;
			CurrentlySeenBuilding->OnEnterPlayerSight();
		}
		else if( !IsValid(CurrentlySeenBuilding) )
		{
			CurrentlySeenBuilding = Building;
			CurrentlySeenBuilding->OnEnterPlayerSight();
		}
	}
	else
	{
		if (IsValid(CurrentlySeenBuilding))
		{
			CurrentlySeenBuilding->OnLeavePlayerSight();
			CurrentlySeenBuilding = nullptr;
		}
	}

}

FTwoVectors  ARPGSystemCharacter::GetLineTracePoints() const
{
	FVector StartLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		StartLocation,
		PlayerViewPointRotation
	);
	StartLocation = GetActorLocation() + FVector(0.f, 0.f, 40.f);
	FVector EndLocation = StartLocation + PlayerViewPointRotation.Vector() * 450;
	return FTwoVectors(StartLocation, EndLocation);
}

//////////////////////////////////////////////////////////////////////////
// Input
void ARPGSystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGSystemCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGSystemCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGSystemCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGSystemCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPGSystemCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPGSystemCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPGSystemCharacter::OnResetVR);
}

void ARPGSystemCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPGSystemCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGSystemCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGSystemCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGSystemCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
