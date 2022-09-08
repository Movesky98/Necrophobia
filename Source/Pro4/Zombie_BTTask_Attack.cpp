// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie_BTTask_Attack.h"
#include "Pro4ZombieAI.h"
#include "Pro4Zombie.h"

UZombie_BTTask_Attack::UZombie_BTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UZombie_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentZombie = Cast<APro4Zombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentZombie == nullptr)
		return EBTNodeResult::Failed;

	CurrentZombie->Attack();
	IsAttacking = true;
	CurrentZombie->OnAttackEnd.AddLambda([this]() -> void
	{
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UZombie_BTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}