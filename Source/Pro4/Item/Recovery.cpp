// Fill out your copyright notice in the Description page of Project Settings.


#include "Recovery.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ARecovery::ARecovery()
{
	ItemType = BaseItemType::Recovery;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sphere(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (SM_Sphere.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_Sphere.Object);
	}
}

void ARecovery::BeginPlay()
{
	Super::BeginPlay();

	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();

	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
	if (WBP_NameWidget != nullptr)
	{
		ItemIconPath = "/Game/UI/Sprites/Weapon_Icon/SMG11_Icon_500x500";
		ItemName = "EncroachMedicine";
		WBP_NameWidget->SetItemName(ItemName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND WBP_NAMEWIDGET"));
	}
}

void ARecovery::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void ARecovery::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void ARecovery::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void ARecovery::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARecovery, ItemIconPath);
	DOREPLIFETIME(ARecovery, ItemName);
	DOREPLIFETIME(ARecovery, ItemNum);
}