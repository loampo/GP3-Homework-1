// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API APickupWeapon : public AGun
{
	GENERATED_BODY()

public:
	bool JustPickedUp = false;
	APickupWeapon();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionSphere;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};