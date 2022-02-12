// Fill out your copyright notice in the Description page of Project Settings.


#include "AArmor.h"

AAArmor::AAArmor()
{
	ItemType = BaseItemType::Armor;

	RandomSpawn();
}

void AAArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Armor is overlapping."));
}

void AAArmor::RandomSpawn()
{
	int32 Random = FMath::RandRange(0, 1);

	CurrentArmor = static_cast<ArmorType>(Random);

	switch (CurrentArmor)
	{
	case AAArmor::ArmorType::Helmet:
	{
		UE_LOG(Pro4, Log, TEXT("Helmet(Armor) is spawned."));
	}
		break;
	case AAArmor::ArmorType::Flak_Jacket:
	{
		UE_LOG(Pro4, Log, TEXT("Flak_Jacket(Armor) is spawned."));
	}
		break;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Armor(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_Y.SM_Scope_25x56_Y"));
	if (SM_Armor.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Armor.Object);
	}
}
