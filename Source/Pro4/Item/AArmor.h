// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"
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
		Helmet,			// Çï¸ä
		Flak_Jacket,	// ¹æÅºÁ¶³¢
		MAX
	};
	
	void ViewItemName();

#pragma region Get_Set

	/* Current_AP */
	void SetCurrentAP(float _AP)
	{
		if (_AP >= 100.0f)
		{
			Current_AP = 100.0f;
		}
		else
		{
			Current_AP = _AP;
		}
	}

	float GetCurrentAP()
	{
		return Current_AP;
	}

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

	/* SK_WeaponItem */
	void SetSKItem(USkeletalMesh* SK_Armor)
	{
		SK_Item = SK_Armor;
		SK_Mesh->SetSkeletalMesh(SK_Item);
	}

	USkeletalMesh* GetSKItem()
	{
		return SK_Item;
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

	UPROPERTY(Replicated)
	float Current_AP;

	float Max_AP = 100.0f;

	USkeletalMesh* SK_Item;

	class UItemNameWidget* WBP_NameWidget;

	bool bIsObservable;
};
