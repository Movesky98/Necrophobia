// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AAmmo::AAmmo()
{
	ItemType = BaseItemType::Ammo;

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45"));
	if (SM_Cube.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Cube.Object);
		BoxMesh->SetRelativeScale3D(FVector(10.0f));
	}
}


void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();

	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
	if (WBP_NameWidget != nullptr)
	{
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/AR4_Icon_500x500";
		ItemName = "Ammo";
		ItemNum = 30;
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
	}
}

void AAmmo::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void AAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void AAmmo::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void AAmmo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAmmo, ItemIconPath);
	DOREPLIFETIME(AAmmo, ItemName);
	DOREPLIFETIME(AAmmo, ItemNum);
}