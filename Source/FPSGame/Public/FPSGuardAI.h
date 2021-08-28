// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSGuardAI.generated.h"

UCLASS()
class FPSGAME_API AFPSGuardAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSGuardAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
