// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawner.h"
#include "Pro4Zombie.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	ZombieSpawnerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ZombieSpawner"));

	ZombieSpawnerComponent->InitSphereRadius(50.0f);
	ZombieSpawnerComponent->SetCollisionProfileName(TEXT("ZombieSpawner"));

	RootComponent = ZombieSpawnerComponent;

	Zombie = APro4Zombie::StaticClass();

	Tags.Add("ZombieSpawner");
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

/* 플레이어의 콜리전에 스포너가 들어올 때 실행되는 함수 */
void AZombieSpawner::PlayerOverlapToZSpawner(APawn* PlayerInstigator)
{
	// 이미 플레이어가 지정되어 있으면 함수를 실행하지 않음.
	if (OverlapPlayer != nullptr && OverlapPlayer != PlayerInstigator)
	{
		return;
	}

	// TODO : SetTimer On Server.
	SpawnZombieDel.BindUFunction(this, FName("SpawnZombieOnServer"), PlayerInstigator);
	GetWorldTimerManager().SetTimer(SpawnZombieTimer, SpawnZombieDel, 2.0f, true);

	bIsPlayerOverlap = true;
	OverlapPlayer = PlayerInstigator;
}

/* 플레이어의 콜리전에 스포너가 멀어졌을 때 실행되는 함수 */
void AZombieSpawner::PlayerAwayFromSpawner(APawn* PlayerInstigator)
{
	if (OverlapPlayer == nullptr || OverlapPlayer != PlayerInstigator)
	{
		return;
	}

	OverlapPlayer = nullptr;
	// TODO : ClearTimer On Server
	GetWorldTimerManager().ClearTimer(SpawnZombieTimer);
}

/* 서버에게 좀비를 스폰해달라 요청하는 함수 */
void AZombieSpawner::SpawnZombieOnServer_Implementation(APawn* PlayerInstigator)
{
	if (!bIsSpawn)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;	// Server
			SpawnParams.Instigator = PlayerInstigator;	// Target Player

			APro4Zombie* SpawnZombie = World->SpawnActor<APro4Zombie>(Zombie, SpawnLocation, SpawnRotation, SpawnParams);
		}

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Server Spawned Zombie."));
		bIsSpawn = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Zombie already has spawned"));
	}
}

bool AZombieSpawner::SpawnZombieOnServer_Validate(APawn* PlayerInstigator)
{
	return true;
}
