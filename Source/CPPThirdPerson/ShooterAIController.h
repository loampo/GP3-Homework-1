// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

private:
	/*UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200;*/
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviour;

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds);

public:
	//virtual void Tick(float DeltaSeconds) override;
	bool IsDead() const;
};
