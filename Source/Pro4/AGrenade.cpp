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
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		if (SM_Grenade.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Grenade.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Flash:
	{
		UE_LOG(Pro4, Log, TEXT("Flash is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		if (SM_Flash.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Flash.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Smoke:
	{
		UE_LOG(Pro4, Log, TEXT("Smoke is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		if (SM_Smoke.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Smoke.Object);
		}
	}
		break;
	case AAGrenade::GrenadeType::Molotov:
	{
		UE_LOG(Pro4, Log, TEXT("Molotov is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		if (SM_Smoke.Succeeded())
		{
			BoxMesh->SetStaticMesh(SM_Smoke.Object);
		}
	}
		break;
	}
}
