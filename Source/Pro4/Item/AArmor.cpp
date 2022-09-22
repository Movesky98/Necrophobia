// Fill out your copyright notice in the Description page of Project Settings.

#include "AArmor.h"

#include "Net/UnrealNetwork.h"

AAArmor::AAArmor()
{
	ItemType = BaseItemType::Armor;
	SetReplicates(true);

	if (HasAuthority())
	{
		RandomItemNum = static_cast<int32>(ArmorType::MAX) - 1;
		RandomItemNum = FMath::RandRange(0, RandomItemNum);
		RandomSpawn(RandomItemNum);
	}
}

void AAArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Armor is overlapping."));
}

void AAArmor::RandomSpawn(int32 Random)
{
	CurrentArmor = static_cast<ArmorType>(Random);

	switch (CurrentArmor)
	{
	case AAArmor::ArmorType::Helmet:
	{
		UE_LOG(Pro4, Log, TEXT("Helmet(Armor) is spawned."));
		ItemName = "Helmet";
		ItemNum = 1;
	}
		break;
	case AAArmor::ArmorType::Flak_Jacket:
	{
		UE_LOG(Pro4, Log, TEXT("Flak_Jacket(Armor) is spawned."));
		ItemName = "Flak_Jacket";
		ItemNum = 1;
	}
		break;
	default:
		UE_LOG(Pro4, Log, TEXT("Armor Spawn ERROR"));
		break;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Armor(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_Y.SM_Scope_25x56_Y"));
	if (SM_Armor.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Armor.Object);
	}
}

void AAArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAArmor, RandomItemNum);
}