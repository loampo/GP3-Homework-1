// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_RespawnAI.generated.h"

/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API UMyBTTask_RespawnAI : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMyBTTask_RespawnAI();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
