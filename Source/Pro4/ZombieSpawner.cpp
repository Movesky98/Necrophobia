// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawner.h"
#include "Pro4Zombie.h"
#include "Pro4Character.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
	bReplicates = true;
	
	ZombieSpawnerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ZombieSpawner"));

	ZombieSpawnerComponent->InitSphereRadius(50.0f);
	ZombieSpawnerComponent->SetCollisionProfileName(TEXT("ZombieSpawner"));

	RootComponent = ZombieSpawnerComponent;

	Zombie = APro4Zombie::StaticClass();

	Tags.Add("ZombieSpawner");
}

/* 월드에 생성되었을 때, 실행되는 함수 */
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

}

/* 플레이어의 콜리전에 스포너가 들어올 때 실행되는 함수 */
void AZombieSpawner::PlayerOverlapToZSpawner_Implementation(APawn* PlayerInstigator)
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
void AZombieSpawner::PlayerAwayFromSpawner_Implementation(APawn* PlayerInstigator)
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
	APro4Character* PlayerCharacter = Cast<APro4Character>(OverlapPlayer);

	if (!bIsSpawn && PlayerCharacter->GetSpawnZombieCurCount() < PlayerCharacter->GetSpawnZombieMaxCount())
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;	// Server
			SpawnParams.Instigator = PlayerInstigator;	// Target Player
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, PlayerInstigator->GetName());

			APro4Zombie* SpawnZombie = World->SpawnActor<APro4Zombie>(Zombie, SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnZombie)
			{
				SpawnZombie->SetZombieTarget(PlayerInstigator);

				PlayerCharacter->SetSpawnZombieCurCount(PlayerCharacter->GetSpawnZombieCurCount() + 1);
			}
		}

		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Server Spawned Zombie."));
		bIsSpawn = true;
	}
	else
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Zombie already has spawned"));
	}
}

/* 서버에게 좀비를 스폰해달라 했을 때, 서버에서 검증하는 함수 */
bool AZombieSpawner::SpawnZombieOnServer_Validate(APawn* PlayerInstigator)
{
	return true;
}
