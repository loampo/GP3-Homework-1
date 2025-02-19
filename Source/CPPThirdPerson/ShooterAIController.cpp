// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviour == nullptr) return;

	RunBehaviorTree(AIBehaviour);
	if (GetPawn() != nullptr) GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(TEXT("FlagLocation"), FVector(1320.0f, 1990.0f, 200.0f)); //Momentaneamente hardcodato
}


void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsDead()) GetBlackboardComponent()->SetValueAsBool(TEXT("isDead"), true);
}


bool AShooterAIController::IsDead() const
{
	AMyCharacter* ControlledCharacter = Cast<AMyCharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
		return ControlledCharacter->IsDead();

	return true;
}