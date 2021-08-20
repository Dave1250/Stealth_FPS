// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(300.f));

	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(250.f, 250.f, 250.f);
	DecalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		if (PlayerCharacter->bIsCarryingObjective)
		{
			AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->FinishMission(PlayerCharacter);
			}
		}
		else
		{
			UGameplayStatics::PlaySound2D(this, MissionNotOverSound);
		}
	}
}

