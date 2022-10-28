// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "../Pro4Character.h"

#include "Components/Button.h"
#include "Components/Image.h"

bool UInventorySlot::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InventorySlotButton != nullptr)) return false;

	InventorySlotButton->OnClicked.AddDynamic(this, &UInventorySlot::UseInventoryItem);

	return true;
}

void UInventorySlot::SetUp(FString Type, FString Name, uint16 Num, FString Path)
{
	SetItemName(Name);
	SetItemNum(Num);
	SetItemType(Type);
	
	// Image를 그림
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);

	InventorySlotImage->SetBrushFromTexture(ItemImage);

	UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *InventorySlotImage->Brush.GetResourceName().ToString());
}

FString UInventorySlot::GetItemType()
{
	return ItemType;
}

void UInventorySlot::SetItemType(FString _ItemType)
{
	ItemType = _ItemType;
}

FString UInventorySlot::GetItemName()
{
	return ItemName;
}

void UInventorySlot::SetItemName(FString Name)
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

/* 인벤토리에서 아이템을 사용할 경우 실행되는 함수 */
void UInventorySlot::UseInventoryItem()
{
	APro4Character* PlayerCharacter = Cast<APro4Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (GetItemType() == "Vaccine")
	{
		PlayerCharacter->CallHelicopterToEscape();
	}
	else if (GetItemType() == "Recovery")
	{
		PlayerCharacter->RecoveryEncroach();
	}
}
