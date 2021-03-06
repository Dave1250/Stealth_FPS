// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"


AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));

	MovmentSpeed = BasicMovmentSpeed;
	Noise = BasicNoise;
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AFPSCharacter::Dash);
	PlayerInputComponent->BindAction("Stealth", IE_Pressed, this, &AFPSCharacter::StealthMode);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}


void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		float Speed = Value * MovmentSpeed;
		AddMovementInput(GetActorForwardVector(), Speed);
		MakeNoise(Noise, GetInstigator());
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		float Speed = Value * MovmentSpeed;
		AddMovementInput(GetActorRightVector(), Speed);
		MakeNoise(Noise, GetInstigator());
	}
}

void AFPSCharacter::Dash()
{
	if (GetWorld()->GetTimeSeconds() > DashTimer && this->CanJump())
	{
		FVector DashVelocity =  this->GetVelocity() * DashPower;
		this->LaunchCharacter(DashVelocity, true, false);
		DashTimer = GetWorld()->GetTimeSeconds() + DashCD;
	}
}

void AFPSCharacter::StealthMode()
{
	if (bStealthTurned)
	{
		bStealthTurned = false;
		MovmentSpeed = BasicMovmentSpeed;
		Noise = BasicNoise;
	}
	else
	{
		bStealthTurned = true;
		MovmentSpeed = StealthMovmentSpeed;
		Noise = StealthNoise;
	}
	//if (GetWorld()->GetTimeSeconds() > StealthTimer)
	//{
		//StealthTimer = GetWorld()->GetTimeSeconds() + StealthCD;
		//Noise = 0.f;
		//GetWorldTimerManager().ClearTimer(TimerHandle_TurnOffStealth);
		//GetWorldTimerManager().SetTimer(TimerHandle_TurnOffStealth, this, &AFPSCharacter::TurnOffStealthMode, StealthActiveTime, false);
	//}
}

/*
void AFPSCharacter::TurnOffStealthMode()
{
	Noise = 1.f;
	MovmentSpeed = 1.f;
}
*/