// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_RespawnAI.h"

#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_RespawnAI::UMyBTTask_RespawnAI()
{
	NodeName = "RespawnAI";
}

EBTNodeResult::Type UMyBTTask_RespawnAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
/*
	FTimerHandle TimerHandle;
	OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&OwnerComp]()
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDead", false);

	}, 10.0f, false);
	
	*/ //Deprecato

	return EBTNodeResult::Succeeded;
}