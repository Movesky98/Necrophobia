// Fill out your copyright notice in the Description page of Project Settings.


#include "AGrenade.h"

AAGrenade::AAGrenade()
{
	ItemType = BaseItemType::Grenade;

	RandomSpawn();
}

void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Grenade is overlapping."));
}

void AAGrenade::RandomSpawn()
{
	int32 Random = FMath::RandRange(0, 3);
	
	CurrentGrenade = static_cast<GrenadeType>(Random);

	switch (CurrentGrenade)
	{
	case AAGrenade::GrenadeType::Grenade:
	{
		UE_LOG(Pro4, Log, TEXT("Grenade is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/Test/Cube_3.Cube_3"));
		if (SM_Grenade.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Grenade.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Flash:
	{
		UE_LOG(Pro4, Log, TEXT("Flash is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/Test/Cube_2.Cube_2"));
		if (SM_Flash.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Flash.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Smoke:
	{
		UE_LOG(Pro4, Log, TEXT("Smoke is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/Test/Cube.Cube"));
		if (SM_Smoke.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Smoke.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Molotov:
	{
		UE_LOG(Pro4, Log, TEXT("Molotov is spawned."));

	}
		break;
	}
}
