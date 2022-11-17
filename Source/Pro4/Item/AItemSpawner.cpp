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

	ItemSpawnerComponent->InitSphereRadius(1.0f);
	ItemSpawnerComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
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

/* 서버가 아이템을 스폰하도록 하는 함수 */
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

		switch (RandomSpawnNum) {
		case 1:
			// 무기
			World->SpawnActor<AAWeapon>(AAWeapon::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 2:
			// 방어구
			World->SpawnActor<AAArmor>(AAArmor::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 3:
			// 투척 무기
			World->SpawnActor<AAGrenade>(AAGrenade::StaticClass(), SpawnLocation, Rot, SpawnParams);
		break;
		case 4:
			// 회복 아이템
			World->SpawnActor<ARecovery>(ARecovery::StaticClass(), SpawnLocation, Rot, SpawnParams);
			break;
		case 5:
			// 탄약
			World->SpawnActor<AAmmo>(AAmmo::StaticClass(), SpawnLocation, Rot, SpawnParams);
			break;
		default:
			UE_LOG(Pro4, Warning, TEXT("Spawn Item ERROR."));
			return;
			break;
		}
	}
}

/* 게임이 시작되었을 때, 서버에서 백신을 소환하도록 하는 함수 */
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

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAItemSpawner, RandomSpawnNum);
}