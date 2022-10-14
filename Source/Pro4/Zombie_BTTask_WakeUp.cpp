// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie_BTTask_WakeUp.h"
#include "Pro4ZombieAI.h"
#include "Pro4Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UZombie_BTTask_WakeUp::UZombie_BTTask_WakeUp()
{
	bNotifyTick = true;
	IsDowning = false;
}

EBTNodeResult::Type UZombie_BTTask_WakeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentZombie = Cast<APro4Zombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	CurrentZombie->WakeUp();
	
	IsDowning = true;

	CurrentZombie->OnWakeUpEnd.AddLambda([this]() -> void
	{
		IsDowning = false;
	});

	return EBTNodeResult::InProgress;
}

void UZombie_BTTask_WakeUp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsDowning)
	{		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}