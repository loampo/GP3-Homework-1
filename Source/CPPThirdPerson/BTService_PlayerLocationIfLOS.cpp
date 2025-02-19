// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfLOS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "MyCharacter.h"


UBTService_PlayerLocationIfLOS::UBTService_PlayerLocationIfLOS()
{
	NodeName = "Update Player Location If In LOS";
}

void UBTService_PlayerLocationIfLOS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(PlayerPawn);

	if (PlayerPawn == nullptr || PlayerCharacter == nullptr) return;
	if (OwnerComp.GetAIOwner() == nullptr) return;

	// Per far smettere di sparare l'AI quando il player è morto(non sembra funzionare a dovere)
	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && !PlayerCharacter->IsDead())
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	else
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}