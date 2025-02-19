// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

uint16 UBTTask_ClearBlackboardValue::GetInstanceMemorySize() const
{
	return sizeof(MYBTStruct);
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/*MYBTStruct* value = reinterpret_cast<MYBTStruct*>(NodeMemory);
	value->value = 5;
	bNotifyTick = true;*/

	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}

//void UBTTask_ClearBlackboardValue::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//
//	if (NodeMemory == nullptr) return;
//
//	MYBTStruct* value = reinterpret_cast<MYBTStruct*>(NodeMemory);
//
//	UE_LOG(LogTemp, Warning, TEXT("Test Clear Key %d"), (*value).value);
//
//
//}
