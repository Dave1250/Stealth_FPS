// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSJumpPad.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSJumpPad::AFPSJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSJumpPad::OverlapJumpPad);
	
}

// Called when the game starts or when spawned
void AFPSJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSJumpPad::OverlapJumpPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator JumpDirection = GetActorRotation();
	JumpDirection.Pitch += JumpPitchAngle;
	FVector JumpVelocity = JumpDirection.Vector() * JumpStrenght;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(JumpVelocity, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateJumpPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(JumpVelocity, NAME_None, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateJumpPadEffect, GetActorLocation());
	}
}

