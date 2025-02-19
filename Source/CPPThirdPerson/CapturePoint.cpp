// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"

#include "MyCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ACapturePoint::ACapturePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(CaptureRadius);
	//CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // Bloccava i proiettili, non ricordo perchè l'avevamo messo a lezione 
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll")); // Così non blocca i proiettili

}

// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACapturePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckOverlappingActors();

}

void ACapturePoint::CheckOverlappingActors()
{
	
	TArray<AActor*> OverlappingActors;
	CollisionSphere->GetOverlappingActors(OverlappingActors, AMyCharacter::StaticClass());

	int32 PlayerCount = 0;
	int32 AICount = 0;

	//Conteggio necessario per la situazione in cui ci siano sia player che AI o si rompe tutto
	for (AActor* Actor : OverlappingActors)
	{
		AMyCharacter* Character = Cast<AMyCharacter>(Actor);
		if (Character)
		{
			if (Character->isPlayer()) PlayerCount++;
			else AICount++;
		}
	}

	if (PlayerCount > 0 && AICount == 0)
	{
		// Solo player presenti
		CapturePoint(true, false);
		//UE_LOG(LogTemp, Warning, TEXT("Player sta conquistando!"));
	}
	else if (AICount > 0 && PlayerCount == 0)
	{
		// Solo AI presenti
		CapturePoint(false, true);
		//UE_LOG(LogTemp, Warning, TEXT("AI sta conquistando!"));
	}
	else if (AICount == 0 && PlayerCount == 0)
	{
		//Nessuno
		CapturePoint(false, false);
		//UE_LOG(LogTemp, Warning, TEXT("Conflitto o nessuno nel punto!"));
	}
	else if (AICount > 0 && PlayerCount > 0)
	{
		//Conflitto
		CapturePoint(true, true);
		//UE_LOG(LogTemp, Warning, TEXT("Conflitto!"));
	}
}

void ACapturePoint::CapturePoint(bool bIsPlayer, bool bIsAI)
{
	if (bIsPlayer && !bIsAI)
	{
		if (CaptureProgress >= CaptureTime && !bIsCaptured)
		{
			bIsCaptured = true;
			CaptureConflict(false);
			UE_LOG(LogTemp, Warning, TEXT("Capture complete!"));
		}
		else if (CaptureProgress < CaptureTime && !bIsCaptured)
		{
			CaptureProgress += CaptureSpeed * 0.2f;
			CaptureConflict(true);
			UE_LOG(LogTemp, Warning, TEXT("Capturing... %f"), CaptureProgress);
		}
	}
	else if (!bIsPlayer && bIsAI)
	{
		if (CaptureProgress <=  -CaptureTime && bIsCaptured)
		{
			bIsCaptured = false;
			CaptureConflict(false);
			UE_LOG(LogTemp, Warning, TEXT("Capture lost!"));
		}
		else if (CaptureProgress > -CaptureTime && bIsCaptured)
		{
			CaptureProgress -= CaptureSpeed * 0.2f;
			CaptureConflict(true);
			UE_LOG(LogTemp, Warning, TEXT("Losing capture... %f"), CaptureProgress);
		}
	}
	else if (bIsPlayer && bIsAI)
	{
		// Conflitto
		CaptureConflict(true);
		UE_LOG(LogTemp, Warning, TEXT("Conflitto!"));
	}
	else if (!bIsPlayer && !bIsAI && bIsConflict)
	{
		// Nessuno
		CaptureConflict(false);
		UE_LOG(LogTemp, Warning, TEXT("Nessuno!"));
	}
}

void ACapturePoint::CaptureConflict(bool Conflict)
{
	bIsConflict = Conflict;
}

bool ACapturePoint::IsConflict()
{
	return bIsConflict;
}

bool ACapturePoint::IsCaptured()
{
	return bIsCaptured;
}

