// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "AArmor.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AAArmor : public AABaseItem
{
	GENERATED_BODY()
	
public:

	AAArmor();

	enum class ArmorType : uint8
	{
		Helmet,		// ���
		Flak_Jacket	// ��ź����
	};

	ArmorType CurrentArmor;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void RandomSpawn(int32 Random);
};
