// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAWeapon::AAWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Weapon;
	AccMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Accesory"));

	// 여기서 GetLocalRole()을 실행하게 될 경우, Authority를 획득하게 됨.
	
	int32 RandomNum = FMath::RandRange(0, static_cast<int32>(WeaponType::MAX) - 1);
	RandomSpawn(RandomNum);

	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	DrawDebugString(GetWorld(), FVector(0, 0, 50), ItemName, this, FColor::Green, DeltaTime);
}

void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SK_WeaponItem, TemporaryName, ItemIconPath, WeaponBoxImagePath, 1);
	}
}

/* 클라이언트들에게 아이템 정보를 뿌려줌 */
void AAWeapon::NetMulticast_SetUp_Implementation(USkeletalMesh* SK_Weapon, const FString& _ItemName, const FString& _IconPath, const FString& _ImagePath, uint16 _ItemNum)
{
	// 스코프 메쉬와 무기 메쉬 설정
	if (WBP_NameWidget == nullptr)
	{
		return;
	}

	SK_Mesh->SetSkeletalMesh(SK_Weapon);
	ItemName = _ItemName;
	WBP_NameWidget->SetItemName(ItemName);
	ItemIconPath = _IconPath;
	WeaponBoxImagePath = _ImagePath;
	ItemNum = _ItemNum;

	if (SK_WeaponSight != nullptr)
	{
		AccMesh->SetStaticMesh(SK_WeaponSight);
		if (SK_Mesh->DoesSocketExist("b_gun_scopeSocket"))
		{
			AccMesh->AttachToComponent(SK_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "b_gun_scopeSocket");
		}
	}
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_Scope(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_T4_Sight.SM_T4_Sight"));
		if (SK_Scope.Succeeded())
		{
			SK_WeaponSight = SK_Scope.Object;
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

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_Scope(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_Y.SM_Scope_25x56_Y"));
		if (SK_Scope.Succeeded())
		{
			SK_WeaponSight = SK_Scope.Object;
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