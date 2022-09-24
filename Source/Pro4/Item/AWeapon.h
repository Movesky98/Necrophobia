// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "AWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AAWeapon : public AABaseItem
{
	GENERATED_BODY()
	
public:

	AAWeapon();

	enum class WeaponType : int32
	{
		AR,
		SR,
		Pistol,
		Knife,
		MAX
	};
	
	WeaponType CurrentWeapon;
	FString TemporaryName;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void RandomSpawn(int32 Random);

	UStaticMesh* SM_WeaponItem;
};
