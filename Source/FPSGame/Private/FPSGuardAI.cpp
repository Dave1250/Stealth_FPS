// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGuardAI.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFPSGuardAI::AFPSGuardAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSGuardAI::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSGuardAI::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSGuardAI::OnNoiseHeard);

	StartRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

// Called every frame
void AFPSGuardAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float Distance = Delta.Size();

		if (Distance < 100)
		{
			MoveToNextPatrolPoint();
		}
	}

}

void AFPSGuardAI::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 45.f, 12, FColor::Yellow, false, 5.f);
		SetGuardState(EAIState::Alerted);

		AController* TempController = GetController();
		if (TempController)
		{
			TempController->StopMovement();
		}

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
	if (GuardState != EAIState::Alerted)
	{
		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();

		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.f;
		NewLookAt.Roll = 0.f;
		SetActorRotation(NewLookAt);


		SetGuardState(EAIState::Suspicious);



		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);
		FVector Delta = GetActorLocation() - Location;
		float Distance = Delta.Size();
		if (Distance < 100)
		{
			AController* TempController = GetController();
			if (TempController)
			{
				TempController->StopMovement();
			}
		}
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSGuardAI::ResetRotation, 2.5f, false);
	}
}

void AFPSGuardAI::ResetRotation()
{
	if (GuardState != EAIState::Alerted)
	{
		SetActorRotation(StartRotation);
		SetGuardState(EAIState::Idle);
		if (bPatrol)
		{
			MoveToNextPatrolPoint();
		}
	}
}


void AFPSGuardAI::SetGuardState(EAIState NewState)
{
	if (GuardState != NewState)
	{
		GuardState = NewState;
		OnGuardStateChanged(GuardState);
	}
}

void AFPSGuardAI::MoveToNextPatrolPoint()
{
	AActor* LastPatrolPoint;
	if (ThirdPatrolPoint)
	{
		LastPatrolPoint = ThirdPatrolPoint;
	}
	else
	{
		LastPatrolPoint = SecondPatrolPoint;
	}

	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == LastPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else if(CurrentPatrolPoint == FirstPatrolPoint)
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = LastPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}