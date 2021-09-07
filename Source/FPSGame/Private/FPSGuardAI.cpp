// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGuardAI.h"
#include "FPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFPSGuardAI::AFPSGuardAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

}

// Called when the game starts or when spawned
void AFPSGuardAI::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSGuardAI::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSGuardAI::OnNoiseHeard);

	StartRotation = GetActorRotation();
}

// Called every frame
void AFPSGuardAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSGuardAI::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 45.f, 12, FColor::Yellow, false, 5.f);
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->FinishMission(SeenPawn, false);
		}
	}
}

void AFPSGuardAI::OnNoiseHeard(APawn* SoundMaker, const FVector& Location, float Volume)
{
	//DrawDebugSphere(GetWorld(), Location, 100.f, 16, FColor::Green, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;
	SetActorRotation(NewLookAt);


	GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSGuardAI::ResetRotation, 2.5f, false);
}

void AFPSGuardAI::ResetRotation()
{
	SetActorRotation(StartRotation);
}
