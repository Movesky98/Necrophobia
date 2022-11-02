// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AAmmo : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AAmmo();
	void ViewItemName();

	UFUNCTION(NetMulticast, Reliable)
	void SetUpOnClient(UStaticMesh* _AmmoMesh, const FString& _IconPath, const FString& _ItemName, uint16 _ItemNum);

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

	/* Icon Path */
	void SetIconPath(FString _IconPath)
	{
		ItemIconPath = _IconPath;
	}

	FString GetIconPath()
	{
		return ItemIconPath;
	}

	virtual void BeginPlay();

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(Replicated)
		FString ItemName;

	UPROPERTY(Replicated)
		uint16 ItemNum;

	UPROPERTY(Replicated)
		FString ItemIconPath;

	bool bIsObservable;

	UItemNameWidget* WBP_NameWidget;
};
