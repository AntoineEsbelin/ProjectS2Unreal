// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectS2Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Splatoon.h"

//////////////////////////////////////////////////////////////////////////
// AProjectS2Character

AProjectS2Character::AProjectS2Character()
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

	// Life : 

	Life = 100;

	WalkSpeed = 600.0f;
	RunSpeed = 1200.0f;

	// Jump samer : 

	JumpCount = 0;
	JumpHeight = 600.0f;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectS2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("DoubleJump", IE_Pressed, this, &AProjectS2Character::DoubleJump);

	PlayerInputComponent->BindAction("Splatoon", IE_Pressed, this, &AProjectS2Character::PaintBall);

	PlayerInputComponent->BindAction("Pickable", IE_Pressed, this, &AProjectS2Character::Pickable);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectS2Character::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectS2Character::OnStopRun);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectS2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectS2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectS2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectS2Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectS2Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectS2Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProjectS2Character::OnResetVR);
}


void AProjectS2Character::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AProjectS2Character::PaintBall()
{
	UWorld* playerLocation = this->GetWorld();
	//Direction du projectile
	FVector bulletForwardVector = this->GetRootComponent()->GetComponentLocation() + (this->GetActorForwardVector() * 110);

	playerLocation->SpawnActor<ASplatoon>(SpawnPaintBall, bulletForwardVector, this->GetActorRotation());
}

void AProjectS2Character::DoubleJump()
{
	if (JumpCount <= 1) {
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, JumpHeight), false, true);
		JumpCount++;
	}
}

void AProjectS2Character::Pickable()
{
	// Return Hit
	FHitResult Hit;
	FHitResult Picked;
	// Return affichage du LineTrace ( le point de départ )
	FVector LineTraceStart = this->GetActorLocation();
	// Distance du Linetrace
	FVector LineTraceForward = FollowCamera->GetForwardVector() + this->GetActorForwardVector();
	// Fin du Linetrace
	FVector LineTraceEnd = ((LineTraceForward * 1000.f) + LineTraceStart);
	// Paramètres du Linetrace , ignore le player
	FCollisionQueryParams LineTraceParam;
	LineTraceParam.AddIgnoredActor(this);
	// Linetrace detection des objets
	DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Blue, false);
	// Au tout premier objet détecté 
	if (GetWorld()->LineTraceSingleByChannel(Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_PhysicsBody, LineTraceParam)) {
		// Si le joueur n'a pas grab l'objet 
		if (!IsPickable) {
			// Si la collision de l'objet est detectée et qu'on peut le bouger
			if (Hit.bBlockingHit && Hit.GetComponent()->Mobility == EComponentMobility::Movable) {
				// Enlève la physique de l'objet zinedine 
				Hit.Component->SetSimulatePhysics(false);
				// Attache l'objet au joueur zebi
				Hit.Actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				// Déplace l'objet vers le haut
				Hit.Actor->SetActorLocation(Hit.Actor->GetActorLocation() + FVector(0.f, 0.f, 80.f));
				ActorPickable = Hit.GetActor();
				IsPickable = true;
			}
		}
		else {
			UPrimitiveComponent* Zinedine = Cast<UPrimitiveComponent>(ActorPickable->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			Zinedine->SetSimulatePhysics(true);
			ActorPickable->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			IsPickable = false;
		}
	}
}

void AProjectS2Character::OnStartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AProjectS2Character::OnStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

// La mort

void AProjectS2Character::Death()
{
	if (Life <= 0) {

		
		GLog->Log("La mort");
		Life = 0;
		
	}
}

void AProjectS2Character::OnResetVR()
{
	// If ProjectS2 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ProjectS2.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProjectS2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AProjectS2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProjectS2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectS2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectS2Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProjectS2Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
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

void AProjectS2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Death();
}

void AProjectS2Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCount = 0;


}


