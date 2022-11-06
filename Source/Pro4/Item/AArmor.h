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

	/* 방어구 종류 */
	enum class ArmorType : int32
	{
		Helmet,			// 헬멧
		Flak_Jacket,	// 방탄조끼
		MAX
	};
	
	/* 아이템 이름을 보여주는 함수 */
	void ViewItemName();

	/* Server -> Client들에게 스폰된 방어구의 정보를 뿌려줌 */
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetUp(USkeletalMesh* SK_Armor, const FString& _ItemName, uint16 _ItemNum);

#pragma region Get_Set

	/* Current_AP */
	void SetCurrentAP(float _AP)
	{
		if (_AP >= 50.0f)
		{
			Current_AP = 50.0f;
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
		SK_ArmorItem = SK_Armor;
		SK_Mesh->SetSkeletalMesh(SK_ArmorItem);
	}

	USkeletalMesh* GetSKItem()
	{
		return SK_Mesh->SkeletalMesh;
	}

#pragma endregion

	virtual void BeginPlay();

protected:
	/* 콜리전이 겹칠 때 실행되는 함수  */
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	/* 방어구 종류 중, 랜덤으로 방어구의 정보를 설정하는 함수 */
	void RandomSpawn(int32 Random);

	ArmorType CurrentArmor;
	FString TemporaryName;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	UPROPERTY(Replicated, VisibleAnywhere)
	float Current_AP;

	float Max_AP = 50.0f;

	USkeletalMesh* SK_ArmorItem;

	UItemNameWidget* WBP_NameWidget;

	bool bIsObservable;
};
