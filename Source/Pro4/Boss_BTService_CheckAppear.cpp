// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_BTService_CheckAppear.h"
#include "Pro4Character.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBoss_BTService_CheckAppear::UBoss_BTService_CheckAppear()
{
	NodeName = TEXT("CheckAppear");
	Interval = 1.0f;
}

void UBoss_BTService_CheckAppear::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return;

	if (CurrentPawn->CheckAppear())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsAppear")), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsAppear")), false);
	}

	return;
}