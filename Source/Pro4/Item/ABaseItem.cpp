// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AABaseItem::AABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SK_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshItem"));
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshItem"));
	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemNameWidget"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	RootComponent = SK_Mesh;
	BoxMesh->SetupAttachment(SK_Mesh);
	SphereCollision->SetupAttachment(SK_Mesh);
	NameWidget->SetupAttachment(SK_Mesh);

	static ConstructorHelpers::FClassFinder<UItemNameWidget> BP_ItemNameWidget(TEXT("/Game/UI/Item/BP_ItemName"));

	if (BP_ItemNameWidget.Succeeded())
	{
		NameWidget->SetWidgetClass(BP_ItemNameWidget.Class);
	}

	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FVector2D DrawSize;
	DrawSize.Set(100.0f, 50.0f);

	NameWidget->SetDrawSize(DrawSize);
	NameWidget->SetRelativeLocation(FVector(0.0, 0.0, 30.0f));
	SK_Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SK_Mesh->SetCollisionProfileName(TEXT("BaseItem"));
	SK_Mesh->SetIsReplicated(true);

	SphereCollision->InitSphereRadius(200.0f);

	SphereCollision->SetCollisionProfileName(TEXT("BaseItem"));


	Tags.Add("Item");
}

