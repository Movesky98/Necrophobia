// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4BossAI.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

APro4BossAI::APro4BossAI()
{
	// 비헤이비어 트리 불러오기
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/JunJae/Boss_BehaviorTree.Boss_BehaviorTree"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	// 블랙보드 불러오기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/AI/JunJae/Boss_Blackboard.Boss_Blackboard"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

	RepeatInterval = 3.0f;
	bSetControlRotationFromPawnOrientation = false;
}

// 빙의될 때 실행되는 함수
void APro4BossAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &APro4ZombieAI::OnRepeatTimer, RepeatInterval, true);

	ABLOG(Error, TEXT("Boss OnPossess Success"));
	// 최초 위치를 블랙보드에 저장
	if (UseBlackboard(BlackboardData, Blackboard))
	{
		Blackboard->SetValueAsVector(FName(TEXT("PrevPos")), InPawn->GetActorLocation());
		if (RunBehaviorTree(BehaviorTree))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree"));
		}
	}
}

void APro4BossAI::OnUnPossess()
{
	Super::OnUnPossess();
	// 빙의에서 벗어날때 캐릭터와 응답변수 리셋
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void APro4BossAI::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return;

	FNavLocation NextLocation;
	// 현재 위치에서 500f 내의 위치로 이동
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}