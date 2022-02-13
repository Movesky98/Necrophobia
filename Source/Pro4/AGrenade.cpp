// Fill out your copyright notice in the Description page of Project Settings.
#include "AGrenade.h"
#include "Net/UnrealNetwork.h"


AAGrenade::AAGrenade()
{
	ItemType = BaseItemType::Grenade;
	SetReplicates(true);
	RandomSpawn();
}

void AAGrenade::BeginPlay()
{
	Super::BeginPlay();

}

void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Grenade is overlapping."));
}

void AAGrenade::RandomSpawn()
{
	int32 Random = FMath::RandRange(0, 3);

	Random = 0;
	CurrentGrenade = static_cast<GrenadeType>(Random);

	switch (CurrentGrenade)
	{
	case AAGrenade::GrenadeType::Grenade:
	{
		UE_LOG(Pro4, Log, TEXT("Grenade is spawned."));
		UStaticMesh* StaticMeshInstance = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		BoxMesh->SetStaticMesh(StaticMeshInstance);
	}
		break;
	case AAGrenade::GrenadeType::Flash:
	{
		UE_LOG(Pro4, Log, TEXT("Flash is spawned."));
		UStaticMesh* StaticMeshInstance = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		BoxMesh->SetStaticMesh(StaticMeshInstance);
	}
		break;
	case AAGrenade::GrenadeType::Smoke:
	{
		UE_LOG(Pro4, Log, TEXT("Smoke is spawned."));
		UStaticMesh* StaticMeshInstance = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		BoxMesh->SetStaticMesh(StaticMeshInstance);
	}
		break;
	case AAGrenade::GrenadeType::Molotov:
	{
		UE_LOG(Pro4, Log, TEXT("Molotov is spawned."));
		UStaticMesh* StaticMeshInstance = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade.SM_G67_Grenade"));
		BoxMesh->SetStaticMesh(StaticMeshInstance);
	}
		break;
	}
}

void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
