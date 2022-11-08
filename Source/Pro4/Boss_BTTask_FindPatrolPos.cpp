// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_BTTask_FindPatrolPos.h"
#include "Pro4BossAI.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

UBoss_BTTask_FindPatrolPos::UBoss_BTTask_FindPatrolPos()
{
	NodeName = TEXT("BossFindPatrolPos");
}

// ���� ���ƴٴϱ� ���� �̵� ������ ���ϴ� �Լ�
EBTNodeResult::Type UBoss_BTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	// �� ��ġ�� �����ϱ� ���� ����
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	// ���� ��ġ�� �����忡 ���� �ϰ� �̵��� ��ġ�� ������ ������
	FVector PrevPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("PrevPos")));
	FNavLocation NextLocation;

	// ���� ��ġ���� 1500f�ȿ� �ִ� �������� �����ϱ� �̵�
	if (NavSystem->GetRandomPointInNavigableRadius(PrevPos, 1500.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
