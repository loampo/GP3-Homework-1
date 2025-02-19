// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWeapon.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"


APickupWeapon::APickupWeapon() 
{
	// Crea la sfera di collisione
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(50.f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
}

void APickupWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("AAmmoPickup overlapped with: %s"), *OtherActor->GetName());
/*
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character && Character->isPlayer() && !JustPickedUp)
	{
		Character->AddToInventory(this);
		//SetActorHiddenInGame(true); 
		JustPickedUp = true;
	} */
}
