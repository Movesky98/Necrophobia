// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp(FString& Name, uint16 Num, FString Path);

	FString GetItemName();
	void SetItemName(FString& Name);

	uint16 GetItemNum();
	void SetItemNum(uint16 Num);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* InventorySlotButton;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* InventorySlotImage;

	FString ItemName;
	uint16 ItemNum;
};
