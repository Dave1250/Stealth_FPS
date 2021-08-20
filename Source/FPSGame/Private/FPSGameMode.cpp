// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::FinishMission(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);


		TArray<AActor*> ReturnedViewTargets;
		UGameplayStatics::GetAllActorsOfClass(this, ViewPointClass, ReturnedViewTargets);

		//Change viewtarget to 1st valid actor's viewpoint
		if (ReturnedViewTargets.Num() > 0)
		{
			AActor* NewViewTarget = ReturnedViewTargets[0];

			APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
			if (PC)
			{
				PC->SetViewTargetWithBlend(NewViewTarget, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ViewPoint Class not found. Update GameMode class with valid subclass"));
		}
	}

	OnMissionCompleted(InstigatorPawn);

}