// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4ZombieAI.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

APro4ZombieAI::APro4ZombieAI()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/JunJae/Zombie_BehaviorTree.Zombie_BehaviorTree"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/AI/JunJae/Zombie_BlackBoard.Zombie_BlackBoard"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

	RepeatInterval = 3.0f;
	bSetControlRotationFromPawnOrientation = false;

	Target = nullptr;
}

void APro4ZombieAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &APro4ZombieAI::OnRepeatTimer, RepeatInterval, true);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		Blackboard->SetValueAsVector(FName(TEXT("PrevPos")), InPawn->GetActorLocation());
		if (RunBehaviorTree(BehaviorTree))
		{
			;
		}
	}
}

void APro4ZombieAI::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void APro4ZombieAI::OnRepeatTimer()
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

void APro4ZombieAI::SetZombieTarget(APawn* Player)
{		
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, Player->GetName());
	Target = Player;
}