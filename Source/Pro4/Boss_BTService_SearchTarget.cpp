// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_BTService_SearchTarget.h"
#include "Pro4Character.h"
#include "Pro4BossAI.h"
#include "Pro4Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBoss_BTService_SearchTarget::UBoss_BTService_SearchTarget()
{
	NodeName = TEXT("BossSearchTarget");
	Interval = 1.0f;
}

void UBoss_BTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<APro4Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 1500.0f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			APro4Character* PlayerCharacter = Cast<APro4Character>(OverlapResult.GetActor());
			if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), PlayerCharacter);

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
}
