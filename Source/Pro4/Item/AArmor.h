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

	enum class ArmorType : int32
	{
		Helmet,			// «Ô∏‰
		Flak_Jacket,	// πÊ≈∫¡∂≥¢
		MAX
	};

	ArmorType CurrentArmor;
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

	UStaticMesh* SM_ArmorItem;
};
