// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSJumpPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSJumpPad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OverlapJumpPad
		(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UPROPERTY(EditInstanceOnly, Category = "Jump Pad")
		float JumpStrenght = 2000.f;

	UPROPERTY(EditInstanceOnly, Category = "Jump Pad")
		float JumpPitchAngle = 45.f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump Pad")
		UParticleSystem* ActivateJumpPadEffect;


private:


};
