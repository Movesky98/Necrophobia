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

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AVaccine::BeginPlay()
{
	Super::BeginPlay(); 
	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
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

/* 아이템의 이름을 가진 UI를 보여주는 함수 */
void AVaccine::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* 아이템과 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AVaccine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* 아이템과 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AVaccine::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AVaccine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVaccine, ItemIconPath);
	DOREPLIFETIME(AVaccine, ItemName);
	DOREPLIFETIME(AVaccine, ItemNum);
}