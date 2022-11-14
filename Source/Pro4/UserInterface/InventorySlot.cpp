// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "../Pro4Character.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

/* UI 위젯 가져오는데에 문제가 없는지 확인하는 함수 */
bool UInventorySlot::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InventorySlotButton != nullptr)) return false;

	InventorySlotButton->OnClicked.AddDynamic(this, &UInventorySlot::UseInventoryItem);

	return true;
}

/* InventorySlot이 Inventory UI에 올라갈 때 슬롯에 대한 설정을 하는 함수 */
void UInventorySlot::SetUp(FString Type, FString Name, uint16 Num, FString Path)
{
	SetItemName(Name);
	SetItemNum(Num);
	SetItemType(Type);
	FString SlotNumber = FString::FromInt(Num);

	InventorySlotNum->SetText(FText::FromString(SlotNumber));
	
	// Image를 그림
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);

	InventorySlotImage->SetBrushFromTexture(ItemImage);

	UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *InventorySlotImage->Brush.GetResourceName().ToString());
}

/* 슬롯 정보 : ItemType을 가져오는 함수 */
FString UInventorySlot::GetItemType()
{
	return ItemType;
}

/* 슬롯 정보 : ItemType을 저장하는 함수 */
void UInventorySlot::SetItemType(FString _ItemType)
{
	ItemType = _ItemType;
}

/* 슬롯 정보 : ItemName을 가져오는 함수 */
FString UInventorySlot::GetItemName()
{
	return ItemName;
}

/* 슬롯 정보 : ItemName을 저장하는 함수 */
void UInventorySlot::SetItemName(FString Name)
{
	ItemName = Name;
}

/* 슬롯 정보 : ItemNum을 가져오는 함수 */
uint16 UInventorySlot::GetItemNum()
{
	return ItemNum;
}

/* 슬롯 정보 : ItemNum을 가져오는 함수 */
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
		PlayerCharacter->CallHelicopterToEscapeOnServer();
	}
	else if (GetItemType() == "Recovery")
	{
		PlayerCharacter->RecoveryEncroach();
	}
}

/* 아이템 슬롯의 수를 최신화하는 함수 */
void UInventorySlot::UpdateSlotCount()
{
	FString SlotNumber = FString::FromInt(GetItemNum());

	InventorySlotNum->SetText(FText::FromString(SlotNumber));
}
