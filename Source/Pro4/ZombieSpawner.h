// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

/*
* 좀비 스포너 헤더파일
* 목표 : 
* 플레이어가 가지고 있는 스폰 콜라이더 범위 내에 들어올 경우,
* 좀비 스포너의 위치를 기준으로 좀비를 생성하는 것이 목표.
*/
UCLASS()
class PRO4_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();

	void PlayerOverlapToZSpawner(APawn* PlayerInstigator);
	void PlayerAwayFromSpawner(APawn* PlayerInstigator);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Get_Set
	bool GetIsSpawn() {
		return bIsSpawn;
	}

	void SetIsSpawn(bool IsSpawn){
		bIsSpawn = IsSpawn;
	}
#pragma endregion


private:
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnZombieOnServer(APawn* PlayerInstigator);

	UPROPERTY(VisibleDefaultsOnly, Category = "OverlapPlayer")
	APawn* OverlapPlayer = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = ZombieSpawner)
	USphereComponent* ZombieSpawnerComponent;

	FTimerDelegate SpawnZombieDel;
	FTimerHandle SpawnZombieTimer;

	TSubclassOf<class APro4Zombie> Zombie;

	float LoopTimeToSpawn = 1.0f;

	bool bIsPlayerOverlap = false;
	bool bIsSpawn = false;
};
