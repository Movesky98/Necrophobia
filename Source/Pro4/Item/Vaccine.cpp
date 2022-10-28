// Fill out your copyright notice in the Description page of Project Settings.


#include "Vaccine.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AVaccine::AVaccine()
{
	ItemType = BaseItemType::Vaccine;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Game/Military/Demo_content/Geometry/Meshes/1M_Cube"));
	if (SM_Cube.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Cube.Object);
	}
}

void AVaccine::BeginPlay()
{
	Super::BeginPlay(); 
	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();

	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
	if (WBP_NameWidget != nullptr)
	{
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/AR4_Icon_500x500";
		ItemName = "Vaccine";
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NameWidget->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NameWidget->GetUserWidgetObject()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
	}
}

void AVaccine::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void AVaccine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void AVaccine::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void AVaccine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVaccine, ItemIconPath);
	DOREPLIFETIME(AVaccine, ItemName);
	DOREPLIFETIME(AVaccine, ItemNum);
}