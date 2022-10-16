// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "../UserInterface/ItemNameWidget.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAWeapon::AAWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Weapon;
	bReplicates = true;
	bNetLoadOnClient = true;

	// ���⼭ GetLocalRole()�� �����ϰ� �� ���, Authority�� ȹ���ϰ� ��.
	int32 RandomNum = FMath::RandRange(0, static_cast<int32>(WeaponType::MAX) - 1);
	RandomSpawn(RandomNum);

	/* Widget Component���� �������� �̸��� ���� Widget�� ������ */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	// DrawDebugString(GetWorld(), FVector(0, 0, 50), ItemName, this, FColor::Green, DeltaTime);
}

void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{ 
		SetUp();
	}
}

void AAWeapon::SetUp()
{
	SK_Mesh->SetSkeletalMesh(SK_WeaponItem);
	ItemName = TemporaryName;

	if(WBP_NameWidget != nullptr)
	{
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		UE_LOG(Pro4, Warning, TEXT("ItemNameWidget Error"));
	}

	ItemNum = 1;
}

void AAWeapon::ViewWeaponName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void AAWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewWeaponName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void AAWeapon::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewWeaponName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void AAWeapon::RandomSpawn(int32 Random)
{
	CurrentWeapon = static_cast<WeaponType>(Random); 

	switch (CurrentWeapon)
	{
	case WeaponType::AR:
	{
		UE_LOG(Pro4, Log, TEXT("AR is spawned."));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4"));
		if (SK_Weapon.Succeeded())
		{
			SK_WeaponItem = SK_Weapon.Object;
		}
		WeaponBoxImagePath = "/Game/UI/Sprites/Weapon_Icon/AR4_Image";
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/AR4_Icon_500x500";
		TemporaryName = "AR";
	}
		break;
	case WeaponType::SR:
	{
		UE_LOG(Pro4, Log, TEXT("SR is spawned."));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SK_KA_Val_Y"));
		if (SK_Weapon.Succeeded())
		{
			SK_WeaponItem = SK_Weapon.Object;
		}

		WeaponBoxImagePath = "/Game/UI/Sprites/Weapon_Icon/KA_val_Image";
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/KA_val_Icon_500x500";
		TemporaryName = "SR";
	}
		break;
	case WeaponType::Pistol:
	{
		UE_LOG(Pro4, Log, TEXT("Pistol is spawned."));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Y"));
		if (SK_Weapon.Succeeded())
		{
			SK_WeaponItem = SK_Weapon.Object;
		}

		WeaponBoxImagePath = "/Game/UI/Sprites/Weapon_Icon/SMG11_Image";
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/SMG11_Icon_500x500";
		TemporaryName = "Pistol";
	}
		break;
	case WeaponType::Knife:
	{
		UE_LOG(Pro4, Log, TEXT("Knife is spawned."));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SK_M9_Knife_X"));
		if (SK_Weapon.Succeeded())
		{
			SK_WeaponItem = SK_Weapon.Object;
		}

		WeaponBoxImagePath = "/Game/UI/Sprites/Weapon_Icon/M9_Knife_Image";
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/M9_Knife_Icon_500x500";
		TemporaryName = "Knife";
	}
		break;
	}
}

void AAWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAWeapon, WeaponBoxImagePath);
	DOREPLIFETIME(AAWeapon, ItemIconPath);
	DOREPLIFETIME(AAWeapon, ItemNum);
	DOREPLIFETIME(AAWeapon, ItemName);
}