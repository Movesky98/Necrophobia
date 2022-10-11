// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"



void UInventorySlot::SetUp(FString& Name, uint16 Num, FString Path)
{
	SetItemName(Name);
	SetItemNum(Num);
	
	// Image¸¦ ±×¸²
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);

	InventorySlotImage->SetBrushFromTexture(ItemImage);

	UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *InventorySlotImage->Brush.GetResourceName().ToString());
}

FString UInventorySlot::GetItemName()
{
	return ItemName;
}

void UInventorySlot::SetItemName(FString& Name)
{
	ItemName = Name;
}

uint16 UInventorySlot::GetItemNum()
{
	return ItemNum;
}

void UInventorySlot::SetItemNum(uint16 Num)
{
	ItemNum = Num;
}
