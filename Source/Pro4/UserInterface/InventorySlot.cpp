// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "../Pro4Character.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

/* UI ���� �������µ��� ������ ������ Ȯ���ϴ� �Լ� */
bool UInventorySlot::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InventorySlotButton != nullptr)) return false;

	InventorySlotButton->OnClicked.AddDynamic(this, &UInventorySlot::UseInventoryItem);

	return true;
}

/* InventorySlot�� Inventory UI�� �ö� �� ���Կ� ���� ������ �ϴ� �Լ� */
void UInventorySlot::SetUp(FString Type, FString Name, uint16 Num, FString Path)
{
	SetItemName(Name);
	SetItemNum(Num);
	SetItemType(Type);
	FString SlotNumber = FString::FromInt(Num);

	InventorySlotNum->SetText(FText::FromString(SlotNumber));
	
	// Image�� �׸�
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);

	InventorySlotImage->SetBrushFromTexture(ItemImage);

	UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *InventorySlotImage->Brush.GetResourceName().ToString());
}

/* ���� ���� : ItemType�� �������� �Լ� */
FString UInventorySlot::GetItemType()
{
	return ItemType;
}

/* ���� ���� : ItemType�� �����ϴ� �Լ� */
void UInventorySlot::SetItemType(FString _ItemType)
{
	ItemType = _ItemType;
}

/* ���� ���� : ItemName�� �������� �Լ� */
FString UInventorySlot::GetItemName()
{
	return ItemName;
}

/* ���� ���� : ItemName�� �����ϴ� �Լ� */
void UInventorySlot::SetItemName(FString Name)
{
	ItemName = Name;
}

/* ���� ���� : ItemNum�� �������� �Լ� */
uint16 UInventorySlot::GetItemNum()
{
	return ItemNum;
}

/* ���� ���� : ItemNum�� �������� �Լ� */
void UInventorySlot::SetItemNum(uint16 Num)
{
	ItemNum = Num;
}

/* �κ��丮���� �������� ����� ��� ����Ǵ� �Լ� */
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

/* ������ ������ ���� �ֽ�ȭ�ϴ� �Լ� */
void UInventorySlot::UpdateSlotCount()
{
	FString SlotNumber = FString::FromInt(GetItemNum());

	InventorySlotNum->SetText(FText::FromString(SlotNumber));
}
