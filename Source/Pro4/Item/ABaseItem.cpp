// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AABaseItem::AABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	
	RootComponent = BoxMesh;
	SphereCollision->SetupAttachment(BoxMesh);

	BoxMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	BoxMesh->SetCollisionProfileName(TEXT("BaseItem"));
	SphereCollision->InitSphereRadius(200.0f);
	SphereCollision->SetCollisionProfileName(TEXT("BaseItem"));

	ItemName = "BaseItem";
	ItemNum = 0;
	Tags.Add("Item");
}

