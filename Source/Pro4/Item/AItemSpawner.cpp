// Fill out your copyright notice in the Description page of Project Settings.


#include "AItemSpawner.h"
#include "AArmor.h"
#include "AGrenade.h"
#include "AWeapon.h"
#include "Ammo.h"
#include "Recovery.h"
#include "Vaccine.h"

#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAItemSpawner::AAItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ItemSpawnerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSpawner"));

	ItemSpawnerComponent->InitSphereRadius(50.0f);
	ItemSpawnerComponent->SetCollisionProfileName(TEXT("BaseItem"));
	
	RootComponent = ItemSpawnerComponent;
	
	Weapon = AAWeapon::StaticClass();
	Armor = AAArmor::StaticClass();
	Grenade = AAGrenade::StaticClass();
}

// Called when the game starts or when spawned
void AAItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->IsServer())
	{
		if (10.0f < Seconds)
		{
			Seconds = 0.0f;
			Server_SpawnItem();
		}

		Seconds += DeltaTime;
	}
}

void AAItemSpawner::Server_SpawnItem()
{
	if (!GetWorld()->IsServer())
	{
		UE_LOG(Pro4, Warning, TEXT("You are not the server."));
		return;
	}

	RandomSpawnNum = FMath::RandRange(1, 5);
	
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FRotator Rot = FRotator::ZeroRotator;
		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X += FMath::RandRange(500, 1000);
		SpawnLocation.Y += FMath::RandRange(500, 1000);

		switch (RandomSpawnNum) {
		case 1:
			World->SpawnActor<AAWeapon>(AAWeapon::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 2:
			World->SpawnActor<AAArmor>(AAArmor::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 3:
			World->SpawnActor<AAGrenade>(AAGrenade::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 4:
			World->SpawnActor<ARecovery>(ARecovery::StaticClass(), SpawnLocation, Rot, SpawnParams);
			break;
		case 5:
			World->SpawnActor<AAmmo>(AAmmo::StaticClass(), SpawnLocation, Rot, SpawnParams);
			break;
		default:
			UE_LOG(Pro4, Warning, TEXT("Spawn Item ERROR."));
			return;
			break;
		}
	}
}

void AAItemSpawner::SpawnVaccine()
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FRotator Rot = FRotator::ZeroRotator;
		FVector SpawnLocation = GetActorLocation();

		World->SpawnActor<AVaccine>(AVaccine::StaticClass(), SpawnLocation, Rot, SpawnParams);
	}
}

void AAItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAItemSpawner, RandomSpawnNum);
}