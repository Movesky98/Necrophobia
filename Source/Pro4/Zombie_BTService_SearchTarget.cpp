// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_BTService_SearchTarget.h"
#include "Pro4Character.h"
#include "Pro4ZombieAI.h"
#include "Pro4Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UZombie_BTService_SearchTarget::UZombie_BTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.0f;
}

void UZombie_BTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<APro4Zombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 500.0f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
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

				CurrentPawn->ZombieRun();
				return;
			}
		}
		//
		//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
		//CurrentPawn->ZombieRunf();
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	CurrentPawn->ZombieRunf();
}