// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4BossAI.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

APro4BossAI::APro4BossAI()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/JunJae/Boss_BehaviorTree.Boss_BehaviorTree"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/AI/JunJae/Boss_Blackboard.Boss_Blackboard"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

	RepeatInterval = 3.0f;
	bSetControlRotationFromPawnOrientation = false;
}

void APro4BossAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &APro4ZombieAI::OnRepeatTimer, RepeatInterval, true);

	ABLOG(Error, TEXT("Boss OnPossess Success"));
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
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void APro4BossAI::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}