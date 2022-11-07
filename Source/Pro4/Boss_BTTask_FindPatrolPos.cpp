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

// 좀비가 돌아다니기 위해 이동 지점을 정하는 함수
EBTNodeResult::Type UBoss_BTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	// 맵 위치를 지정하기 위한 변수
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	// 현재 위치를 블랙보드에 저장 하고 이동할 위치를 저장할 변수들
	FVector PrevPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("PrevPos")));
	FNavLocation NextLocation;

	// 현재 위치에서 1500f안에 있는 지점으로 랜덤하기 이동
	if (NavSystem->GetRandomPointInNavigableRadius(PrevPos, 1500.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
