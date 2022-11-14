// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

/* 생성자 */
AAmmo::AAmmo()
{
	ItemType = BaseItemType::Ammo;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(AmmoType::MAX) - 1);

	CurrentAmmo = static_cast<AmmoType>(RandomItemNum);

	switch (CurrentAmmo)
	{
	case AAmmo::AmmoType::MainWeaponAmmo:
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Game/Weapon/BulletCase_A"));
		if (SM_Cube.Succeeded())
		{
			SM_Ammo = SM_Cube.Object;
		}
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/Rifle_Bullet_Icon_500x500";

		ItemName = "MainWeaponAmmo";
		ItemNum = 30;
	}
		break;
	case AAmmo::AmmoType::SubWeaponAmmo:
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Game/Weapon/BulletCase_B"));
		if (SM_Cube.Succeeded())
		{
			SM_Ammo = SM_Cube.Object;
		}

		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/Pistol_Bullet_Icon_500x500";

		ItemName = "SubWeaponAmmo";
		ItemNum = 30;
	}
		break;
	case AAmmo::AmmoType::MAX:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Ammo Spawn ERROR!"));
		break;
	default:
		break;
	}
}

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());

	if (GetWorld()->IsServer())
	{
		SetUpOnClient(SM_Ammo, ItemIconPath, ItemName, ItemNum);
	}
}

/* Server -> Client들에게 생성된 탄약 정보를 뿌려주는 함수 */
void AAmmo::SetUpOnClient_Implementation(UStaticMesh* _AmmoMesh, const FString& _IconPath, const FString& _ItemName, uint16 _ItemNum)
{
	if (WBP_NameWidget == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
		return;
	}

	BoxMesh->SetStaticMesh(_AmmoMesh);
	ItemIconPath = _IconPath;
	ItemName = _ItemName;
	ItemNum = _ItemNum;
	WBP_NameWidget->SetItemName(ItemName);
}

/* 아이템의 이름을 가진 UI를 보여주는 함수 */
void AAmmo::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* 아이템과 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* 아이템과 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AAmmo::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAmmo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAmmo, ItemIconPath);
	DOREPLIFETIME(AAmmo, ItemName);
	DOREPLIFETIME(AAmmo, ItemNum);
}