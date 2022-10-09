// Fill out your copyright notice in the Description page of Project Settings.


#include "EncroachSpawner.h"
#include "EncroachField.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEncroachSpawner::AEncroachSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ItemSpawnerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EncroachSpawner"));

	ItemSpawnerComponent->InitSphereRadius(50.0f);
	RootComponent = ItemSpawnerComponent;

	EncroachField = AEncroachField::StaticClass();
}

// Called when the game starts or when spawned
void AEncroachSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEncroachSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (5.0f < Seconds)
		{
			Seconds = 0.0f;
			Server_SpawnItem();
		}

		Seconds += DeltaTime;
	}
}

void AEncroachSpawner::Server_SpawnItem()
{
	UE_LOG(Pro4, Log, TEXT("Spawn Field"));
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FRotator Rot;
		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X += FMath::RandRange(500, 5000);
		SpawnLocation.Y += FMath::RandRange(500, 5000);
		AEncroachField* Instance = World->SpawnActor<AEncroachField>(AEncroachField::StaticClass(), SpawnLocation, Rot, SpawnParams);
	}
}

void AEncroachSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEncroachSpawner, RandomSpawnNum);
}