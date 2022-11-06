// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAWeapon::AAWeapon()
{
	ItemType = BaseItemType::Weapon;
	AccMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Accesory"));

	AccMesh->SetCollisionProfileName(TEXT("BaseItem"));
	AccMesh->SetIsReplicated(true);

	// 여기서 GetLocalRole()을 실행하게 될 경우, Authority를 획득하게 됨.
	
	int32 RandomNum = FMath::RandRange(0, static_cast<int32>(WeaponType::MAX) - 1);
	RandomSpawn(RandomNum);

	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SK_WeaponItem, SK_WeaponSight, TemporaryName, ItemIconPath, WeaponBoxImagePath, 1);
	}
}

/* 클라이언트들에게 아이템 정보를 뿌려줌 */
void AAWeapon::NetMulticast_SetUp_Implementation(USkeletalMesh* SK_Weapon, UStaticMesh* SM_Scope, const FString& _ItemName, const FString& _IconPath, const FString& _ImagePath, uint16 _ItemNum)
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

	AccMesh->SetStaticMesh(SM_Scope);
	if (SK_Mesh->DoesSocketExist("b_gun_scopeSocket"))
	{
		AccMesh->AttachToComponent(SK_Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "b_gun_scopeSocket");
	}
}

/* 아이템의 이름을 가진 UI를 보여주는 함수 */
void AAWeapon::ViewWeaponName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* 아이템과 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AAWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewWeaponName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* 아이템과 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AAWeapon::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewWeaponName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* 무기 종류 중, 랜덤으로 무기의 정보를 설정하는 함수 */
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

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAWeapon, WeaponBoxImagePath);
	DOREPLIFETIME(AAWeapon, ItemIconPath);
	DOREPLIFETIME(AAWeapon, ItemNum);
	DOREPLIFETIME(AAWeapon, ItemName);
}