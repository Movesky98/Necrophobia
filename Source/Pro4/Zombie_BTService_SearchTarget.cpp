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

	if (CurrentPawn->ZombieDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("Dead")), true);
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	APro4ZombieAI* ZombieAI = Cast<APro4ZombieAI>(OwnerComp.GetAIOwner());

	if (ZombieAI->GetZombieTarget() != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), ZombieAI->GetZombieTarget());
		ZombieAI->SetIsTracking(true);
	}
	else
	{
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);

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
					ZombieAI->SetZombieTarget(PlayerCharacter);
					ZombieAI->SetIsTracking(true);

					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), PlayerCharacter);

					DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);

					CurrentPawn->ZombieRun();
					return;
				}
			}

			//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
			//CurrentPawn->ZombieRunf();
		}
	}

	if (!CurrentPawn->ZombieDowning())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("Sleep")), true);
	}

	if (!ZombieAI->GetIsTracking())
	{
		CurrentPawn->ZombieRunf();
	}
	else {
		CurrentPawn->ZombieRun();
	}
	
}