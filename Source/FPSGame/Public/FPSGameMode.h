// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();

	void FinishMission(APawn* InstigatorPawn, bool bMissionSuccessed);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccessed);

private:

	UPROPERTY(EditDefaultsOnly, Category = "ViewPoint")
	TSubclassOf<AActor> ViewPointClass;
};



