// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSGuardAI.generated.h"


class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
};

UCLASS()
class FPSGAME_API AFPSGuardAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSGuardAI();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* SoundMaker, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetRotation();

	FRotator StartRotation;

	FTimerHandle TimerHandle_ResetRotation;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnGuardStateChanged(EAIState NewState);

	EAIState GuardState;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* SecondPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* ThirdPatrolPoint;

		AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();
};
