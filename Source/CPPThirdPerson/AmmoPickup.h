// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class CPPTHIRDPERSON_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 AmmoAmount = 10;
	UPROPERTY(EditAnywhere, Category = "Ammo")
	float RespawnTime = 60.f;
	bool JustPickedUp = false;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;


	void Respawn();
	FTimerHandle RespawnTimerHandle;

};
