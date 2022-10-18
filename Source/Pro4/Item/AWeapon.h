// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"
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

	void ViewWeaponName();

	UFUNCTION(Server, Reliable)
	void Server_NoticeToServerSetUpItem(USkeletalMesh* SK_Weapon, const FString& _ItemName, uint16 _ItemNum);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetUp(USkeletalMesh* SK_Weapon, const FString& _ItemName, uint16 _ItemNum);

#pragma region Get_Set

	/* Weapon Box Image Path */
	void SetBoxImagePath(FString _BoxImagePath)
	{
		WeaponBoxImagePath = _BoxImagePath;
	}

	FString GetBoxImagePath()
	{
		return WeaponBoxImagePath;
	}

	/* Icon Path */
	void SetIconPath(FString _IconPath)
	{
		ItemIconPath = _IconPath;
	}

	FString GetIconPath()
	{
		return ItemIconPath;
	}

	/* SK_WeaponItem */
	void SetSKWeaponItem(USkeletalMesh* SK_Weapon)
	{
		SK_WeaponItem = SK_Weapon;
		SK_Mesh->SetSkeletalMesh(SK_WeaponItem);
	}

	USkeletalMesh* GetSKWeaponItem()
	{
		return SK_Mesh->SkeletalMesh;
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

#pragma endregion

	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	void RandomSpawn(int32 Random);
	
	WeaponType CurrentWeapon;

	FString TemporaryName;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	UPROPERTY(Replicated)
	FString ItemIconPath;
	
	UPROPERTY(Replicated)
	FString WeaponBoxImagePath;

	USkeletalMesh* SK_WeaponItem;
	UStaticMesh* SM_WeaponItem;

	class UItemNameWidget* WBP_NameWidget;
	
	bool bIsObservable;
};
