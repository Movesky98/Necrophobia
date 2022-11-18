// Fill out your copyright notice in the Description page of Project Settings.


#include "Vaccine.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AVaccine::AVaccine()
{
	ItemType = BaseItemType::Vaccine;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Game/DownloadAssets/First_Aid_Health_Kit/Meshes/SM_EnergySyringe1_01"));
	if (SM_Cube.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Cube.Object);
	}

	BoxMesh->SetRelativeScale3D(FVector(4.0f));
}

/* �������� ���忡 �����Ǿ��� ��, ����Ǵ� �Լ� */
void AVaccine::BeginPlay()
{
	Super::BeginPlay(); 
	/* Widget Component���� �������� �̸��� ���� Widget�� ������ */
	NameWidget->InitWidget();

	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
	if (WBP_NameWidget != nullptr)
	{
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/Vaccine_Icon_500x500";
		ItemName = "Vaccine";
		ItemNum = 1;
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
	}
}

/* �������� �̸��� ���� UI�� �����ִ� �Լ� */
void AVaccine::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* �����۰� ��ġ�� �����ϴ� ���Ͱ� ���� �� ����Ǵ� �Լ�*/
void AVaccine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* �����۰� ��ġ�� �ִ� ���Ͱ� ��� �� ����Ǵ� �Լ�*/
void AVaccine::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* �����ۿ��� ������ Ŭ���̾�Ʈ�� �����Ǵ� �������� �����ϴ� �Լ� */
void AVaccine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVaccine, ItemIconPath);
	DOREPLIFETIME(AVaccine, ItemName);
	DOREPLIFETIME(AVaccine, ItemNum);
}