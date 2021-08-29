// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGuardAI.h"
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
		DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 45.f, 12, FColor::Yellow, false, 5.f);
	}
}

void AFPSGuardAI::OnNoiseHeard(APawn* SoundMaker, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 100.f, 16, FColor::Green, false, 10.f);

}

