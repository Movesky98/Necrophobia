// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"

AAWeapon::AAWeapon() 
{
	ItemType = BaseItemType::Weapon;

	RandomSpawn();
}

void AAWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Weapon is overlapping."));
}

void AAWeapon::RandomSpawn()
{
	int32 Random = FMath::RandRange(0, 3);
	CurrentWeapon = static_cast<WeaponType>(Random);

	switch (CurrentWeapon)
	{
	case AAWeapon::WeaponType::AR:
	{
		UE_LOG(Pro4, Log, TEXT("AR is spawned."));
	}
		break;
	case AAWeapon::WeaponType::SR:
	{
		UE_LOG(Pro4, Log, TEXT("SR is spawned."));
	}
		break;
	case AAWeapon::WeaponType::Pistol:
	{
		UE_LOG(Pro4, Log, TEXT("Pistol is spawned."));
	}
		break;
	case AAWeapon::WeaponType::Knife:
	{
		UE_LOG(Pro4, Log, TEXT("Knife is spawned."));
	}
		break;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Test/Cube.Cube"));
	if (SM_Weapon.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Weapon.Object);
	}
}
