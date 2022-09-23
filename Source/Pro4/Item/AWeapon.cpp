// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAWeapon::AAWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Weapon;
	bReplicates = true;
	bNetLoadOnClient = true;

	// 여기서 GetLocalRole()을 실행하게 될 경우, Authority를 획득하게 됨.
	int32 RandomNum = FMath::RandRange(0, static_cast<int32>(WeaponType::MAX) - 1);
	RandomSpawn(RandomNum);
}

void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Actor Role Draw
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumRole(GetLocalRole()), this, FColor::Red, DeltaTime);
	// ItemName Draw
	DrawDebugString(GetWorld(), FVector(0, 0, 50), ItemName, this, FColor::Green, DeltaTime);
}

void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{ 
		BoxMesh->SetStaticMesh(SM_WeaponItem);
		ItemName = TemporaryName;
		ItemNum = 1;
	}
}


void AAWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Weapon is overlapping."));
}

void AAWeapon::RandomSpawn(int32 Random)
{
	CurrentWeapon = static_cast<WeaponType>(Random); 

	switch (CurrentWeapon)
	{
	case AAWeapon::WeaponType::AR:
	{
		UE_LOG(Pro4, Log, TEXT("AR is spawned."));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SM_AR4_X"));
		if (SM_Weapon.Succeeded())
		{
			SM_WeaponItem = SM_Weapon.Object;
		}

		TemporaryName = "AR";
	}
		break;
	case AAWeapon::WeaponType::SR:
	{
		UE_LOG(Pro4, Log, TEXT("SR is spawned."));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SM_KA_Val.SM_KA_Val"));
		if (SM_Weapon.Succeeded())
		{
			SM_WeaponItem = SM_Weapon.Object;
		}

		TemporaryName = "SR";
	}
		break;
	case AAWeapon::WeaponType::Pistol:
	{
		UE_LOG(Pro4, Log, TEXT("Pistol is spawned."));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SM_SMG11.SM_SMG11"));
		if (SM_Weapon.Succeeded())
		{
			SM_WeaponItem = SM_Weapon.Object;
		}

		TemporaryName = "Pistol";
	}
		break;
	case AAWeapon::WeaponType::Knife:
	{
		UE_LOG(Pro4, Log, TEXT("Knife is spawned."));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SM_M9_Knife.SM_M9_Knife"));
		if (SM_Weapon.Succeeded())
		{
			SM_WeaponItem = SM_Weapon.Object;
		}

		TemporaryName = "Knife";
	}
		break;
	}
}

void AAWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAWeapon, ItemNum);
	DOREPLIFETIME(AAWeapon, ItemName);
}
