// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CPPTHIRDPERSON_API APressurePlate : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* Trigger;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

public:
	APressurePlate();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
