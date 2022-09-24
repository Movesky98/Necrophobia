// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"



void UInventorySlot::SetUp(FString& Name, uint16 Num)
{
	SetItemName(Name);
	SetItemNum(Num);
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
