// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapturePoint.generated.h"

UCLASS()
class CPPTHIRDPERSON_API ACapturePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapturePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StopCapture();
	void CheckOverlappingActors();
	bool IsConflict(bool bIsConflict);
	bool IsCaptured();
	bool IsConflict();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	

	UPROPERTY(EditAnywhere, Category = "Capture Point")
	bool bIsCaptured = false;
	UPROPERTY(EditAnywhere, Category = "Capture Point")
	float CaptureTime = 10.f;
	UPROPERTY(EditAnywhere, Category = "Capture Point")
	float CaptureProgress = 0.f;
	UPROPERTY(EditAnywhere, Category = "Capture Point")
	float CaptureSpeed = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Capture Point")
	bool bIsPlayerControlled = false;
	UPROPERTY(EditAnywhere, Category = "Capture Point")
	float CaptureRadius = 200.f;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionSphere;

	bool bIsConflict = false;
	
	void CapturePoint(bool bIsPlayer, bool bIsAI);
	void CaptureConflict(bool Conflict);
};
