// Fill out your copyright notice in the Description page of Project Settings.

#include "AGrenade.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"


AAGrenade::AAGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Grenade;
	bReplicates = true;
	bNetLoadOnClient = true;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(GrenadeType::MAX) - 1);
	RandomSpawn(RandomItemNum);
}

void AAGrenade::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{
		BoxMesh->SetStaticMesh(SM_GrenadeItem);
		ItemName = TemporaryName;
		ItemNum = 1;
	}
}

void AAGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	DrawDebugString(GetWorld(), FVector(0, 0, 100), ItemName, this, FColor::Green, DeltaTime);
}

void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Grenade is overlapping."));

}

void AAGrenade::RandomSpawn(int32 Random)
{
	CurrentGrenade = static_cast<GrenadeType>(Random);

	switch (CurrentGrenade)
	{
	case AAGrenade::GrenadeType::Grenade:
	{
		UE_LOG(Pro4, Log, TEXT("Grenade is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade"));
		if (SM_Grenade.Succeeded())
		{
			SM_GrenadeItem = SM_Grenade.Object;
		}
		
		TemporaryName = "Grenade";
	}
		break;
	case AAGrenade::GrenadeType::Flash:
	{
		UE_LOG(Pro4, Log, TEXT("Flash is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade"));
		if (SM_Flash.Succeeded())
		{
			SM_GrenadeItem = SM_Flash.Object;
		}

		TemporaryName = "Flash";
	}
		break;
	case AAGrenade::GrenadeType::Smoke:
	{
		UE_LOG(Pro4, Log, TEXT("Smoke is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade"));
		if (SM_Smoke.Succeeded())
		{
			SM_GrenadeItem = SM_Smoke.Object;
		}

		TemporaryName = "Smoke";
	}
		break;
	case AAGrenade::GrenadeType::Molotov:
	{
		UE_LOG(Pro4, Log, TEXT("Molotov is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Molotov(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Grenade"));
		if (SM_Molotov.Succeeded())
		{
			SM_GrenadeItem = SM_Molotov.Object;
		}

		TemporaryName = "Molotov";
	}
		break;
	}
}

void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAGrenade, ItemName);
	DOREPLIFETIME(AAGrenade, ItemNum);
}