// Fill out your copyright notice in the Description page of Project Settings.


#include "AItemSpawner.h"
#include "Math/UnrealMathUtility.h"
#include "AArmor.h"
#include "AGrenade.h"
#include "AWeapon.h"

// Sets default values
AAItemSpawner::AAItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSpawnerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSpawner"));

	ItemSpawnerComponent->InitSphereRadius(50.0f);
	RootComponent = ItemSpawnerComponent;
	
}

// Called when the game starts or when spawned
void AAItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAItemSpawner::SpawnItem()
{
	UE_LOG(Pro4, Log, TEXT("Spawn Item"));

	int32 Random = FMath::RandRange(1, 3);

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FRotator Rot;
		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X += FMath::RandRange(500, 1000);
		SpawnLocation.Y += FMath::RandRange(500, 1000);
		
		switch (Random) {
		case 1:
		{
			AAWeapon* InstanceItem = World->SpawnActor<AAWeapon>(Weapon, SpawnLocation, Rot, SpawnParams);
		}
			break;
		case 2:
		{
			AAArmor* InstanceItem = World->SpawnActor<AAArmor>(Armor, SpawnLocation, Rot, SpawnParams);
		}
			break;
		case 3:
		{
			AAGrenade* InstanceItem = World->SpawnActor<AAGrenade>(Grenade, SpawnLocation, Rot, SpawnParams);
		}
			break;
		case 4:
			// InstanceItem->ItemType = AABaseItem::BaseItemType::Recovery;
			break;
		case 5:
			// InstanceItem->ItemType = AABaseItem::BaseItemType::Ammo;
			break;
		case 6:
			// InstanceItem->ItemType = AABaseItem::BaseItemType::Parts;
			break;
		default:
			break;
		}

		// InstanceItem->ItemSetting();
	}
}

// Called every frame
void AAItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (5.0f < Seconds)
	{
		Seconds = 0.0f;
		SpawnItem();
	}

	Seconds += DeltaTime;
}

