// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SphereComponent.h"
#include "AmmoPickup.h"
#include "MyCharacter.h"


AAmmoPickup::AAmmoPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));  //Per evitare che gli vada addosso
	//SetActorEnableCollision(false);
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(50.f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
}

void AAmmoPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("AAmmoPickup overlapped with: %s"), *OtherActor->GetName());
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character && Character->isPlayer() && !JustPickedUp && !Character->isFullAmmo())
	{
		Character->AddAmmo(AmmoAmount);
		Character->UpdateAmmoUI();
		SetActorHiddenInGame(true);
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AAmmoPickup::Respawn, RespawnTime, false);
		JustPickedUp = true;
	}
}

void AAmmoPickup::Respawn()
{
	SetActorHiddenInGame(false);
	JustPickedUp = false;
}

