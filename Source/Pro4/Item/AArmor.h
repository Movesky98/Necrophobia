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
	
	void ViewItemName();

#pragma region Get_Set

	/* ItemName */
	void SetItemName(FString _Name)
	{
		ItemName = _Name;
	}

	FString GetItemName()
	{
		return ItemName;
	}

	/* ItemNum */
	void SetItemNum(uint16 _Num)
	{
		ItemNum = _Num;
	}

	uint16 GetItemNum()
	{
		return ItemNum;
	}

#pragma endregion

	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	void RandomSpawn(int32 Random);
	void SetUp();

	ArmorType CurrentArmor;
	FString TemporaryName;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	UStaticMesh* SM_ArmorItem;

	class UItemNameWidget* WBP_NameWidget;

	bool bIsObservable;
};
