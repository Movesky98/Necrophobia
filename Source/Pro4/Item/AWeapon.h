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

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(Replicated)
	int32 RandomItemNum = 0;

	void RandomSpawn(int32 Random);
};
