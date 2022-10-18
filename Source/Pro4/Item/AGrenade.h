// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"
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

	void ViewItemName();


	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetUp(UStaticMesh* SM_Grenade, const FString& _ItemName, uint16 _ItemNum);

#pragma region Get_Set

	/* ItemName */
	void SetItemName(FString _Name)
	{
		ItemName = _Name;
		WBP_NameWidget->SetItemName(ItemName);
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

	GrenadeType CurrentGrenade;

	UStaticMesh* SM_GrenadeItem;

	FString TemporaryName;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	class UItemNameWidget* WBP_NameWidget;

	bool bIsObservable;
};
