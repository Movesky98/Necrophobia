// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "AGrenade.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AAGrenade : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AAGrenade();

	enum class GrenadeType : int32
	{
		Grenade,	// 수류탄
		Flash,		// 섬광탄
		Smoke,		// 연막탄
		Molotov,	// 화염병
		MAX
	};
	
	GrenadeType CurrentGrenade;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(Replicated)
	int32 RandomItemNum = 0;

	void RandomSpawn(int32 Random);
};
