// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

/* ������ */
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

/* �������� ���忡 �����Ǿ��� ��, ����Ǵ� �Լ� */
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	/* Widget Component���� �������� �̸��� ���� Widget�� ������ */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());

	if (GetWorld()->IsServer())
	{
		SetUpOnClient(SM_Ammo, ItemIconPath, ItemName, ItemNum);
	}
}

/* Server -> Client�鿡�� ������ ź�� ������ �ѷ��ִ� �Լ� */
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

/* �������� �̸��� ���� UI�� �����ִ� �Լ� */
void AAmmo::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* �����۰� ��ġ�� �����ϴ� ���Ͱ� ���� �� ����Ǵ� �Լ�*/
void AAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* �����۰� ��ġ�� �ִ� ���Ͱ� ��� �� ����Ǵ� �Լ�*/
void AAmmo::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* �����ۿ��� ������ Ŭ���̾�Ʈ�� �����Ǵ� �������� �����ϴ� �Լ� */
void AAmmo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAmmo, ItemIconPath);
	DOREPLIFETIME(AAmmo, ItemName);
	DOREPLIFETIME(AAmmo, ItemNum);
}