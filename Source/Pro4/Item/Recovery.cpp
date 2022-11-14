// Fill out your copyright notice in the Description page of Project Settings.


#include "Recovery.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

/* ��� ġ���� */
ARecovery::ARecovery()
{
	ItemType = BaseItemType::Recovery;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sphere(TEXT("/Game/DownloadAssets/First_Aid_Health_Kit/Meshes/SM_AidPot1_01"));
	if (SM_Sphere.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Sphere.Object);
	}

	BoxMesh->SetRelativeScale3D(FVector(3.0f));
}

/* �������� ���忡 �����Ǿ��� ��, ����Ǵ� �Լ� */
void ARecovery::BeginPlay()
{
	Super::BeginPlay();

	/* Widget Component���� �������� �̸��� ���� Widget�� ������ */
	NameWidget->InitWidget();

	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
	if (WBP_NameWidget != nullptr)
	{
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/Medicine_Icon_500x500";
		ItemName = "EncroachMedicine";
		ItemNum = 1;
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
	}
}

/* �������� �̸��� ���� UI�� �����ִ� �Լ� */
void ARecovery::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* �����۰� ��ġ�� �����ϴ� ���Ͱ� ���� �� ����Ǵ� �Լ�*/
void ARecovery::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* �����۰� ��ġ�� �ִ� ���Ͱ� ��� �� ����Ǵ� �Լ�*/
void ARecovery::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* �����ۿ��� ������ Ŭ���̾�Ʈ�� �����Ǵ� �������� �����ϴ� �Լ� */
void ARecovery::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARecovery, ItemIconPath);
	DOREPLIFETIME(ARecovery, ItemName);
	DOREPLIFETIME(ARecovery, ItemNum);
}