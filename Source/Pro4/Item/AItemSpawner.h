// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "GameFramework/Actor.h"
#include "AItemSpawner.generated.h"

UCLASS()
class PRO4_API AAItemSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AAItemSpawner();

	void Server_SpawnItem();

	UPROPERTY(VisibleDefaultsOnly, Category = TimeSecond, meta = (AllowPrivateAccess = true))
	float Seconds = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = ItemSpawner)
	USphereComponent* ItemSpawnerComponent;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AAWeapon> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AAArmor> Armor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AAGrenade> Grenade;

private:
	UPROPERTY(Replicated)
	int32 RandomSpawnNum = 0;

	bool isSpawn = false;
};
