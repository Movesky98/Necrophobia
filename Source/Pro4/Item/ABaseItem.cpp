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

	bReplicates = true;
	bNetLoadOnClient = true;
	NetCullDistanceSquared = 2500000000.0f;

	RootComponent = BoxMesh;
	SK_Mesh->SetupAttachment(BoxMesh);
	NameWidget->SetupAttachment(BoxMesh);

	// 경로로부터 Blueprint ItemNameWidget을 가져옴
	static ConstructorHelpers::FClassFinder<UItemNameWidget> BP_ItemNameWidget(TEXT("/Game/UI/Item/BP_ItemName"));
	if (BP_ItemNameWidget.Succeeded())
	{
		NameWidget->SetWidgetClass(BP_ItemNameWidget.Class);
	}

	/* NameWidget 초기 세팅 */
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidget->SetIsReplicated(true);

	FVector2D DrawSize;
	DrawSize.Set(100.0f, 50.0f);
	NameWidget->SetDrawSize(DrawSize);

	/* 아이템의 기본 콜리전 설정 */
	BoxMesh->SetCollisionProfileName(TEXT("BaseItem"));
	BoxMesh->SetIsReplicated(true);

	SK_Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SK_Mesh->SetCollisionProfileName(TEXT("BaseItem"));
	SK_Mesh->SetIsReplicated(true);

	Tags.Add("Item");
}
