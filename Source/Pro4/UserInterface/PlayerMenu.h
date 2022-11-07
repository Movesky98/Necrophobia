// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"

#include "WidgetMenu.h"

#include "PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UPlayerMenu : public UWidgetMenu
{
	GENERATED_BODY()
	
public:
	UPlayerMenu(const FObjectInitializer& ObjectInitializer);

	void SetUp();
	void SetPlayerHP(float CurHP, float MaxHP);
	void SetPlayerAP(float CurAP, float MaxAP);
	void ActiveWeaponShortcut(uint16 SlotNumber);
	void ActiveArmorImage(bool IsHelmet);

	virtual bool Initialize();

	void SetTimeText(uint16 min_, uint16 sec_);
	void SetImage(UTexture2D* InTexture);

	void ChangePlayerWidget();
	void PlayerZoomWidget();

	void AddItemToGrenade(const FString& GrenadeName, uint16 Num);
	void AddItemToWeapon(FString _ImagePath, FString _IconPath, FString _WeaponName);
	void AddItemToInventory(AActor* ItemActor, uint16 Num);

	class UTexture2D* Day;
	class UTexture2D* Night;

private:

	UFUNCTION(Client, Reliable)
	void Client_SetTimeText(uint16 min_, uint16 sec_);

	TSubclassOf<class UInventorySlot> InventorySlot;

	UTexture2D* SlotChoose;
	UTexture2D* SlotEmpty;

	FString SlotItemChoosePath = "/Game/UI/Sprites/Player_UI/Item_block/Item_choose";
	FString SlotItemEmptyPath = "/Game/UI/Sprites/Player_UI/Item_block/Item_empty";

	UPROPERTY(meta = (BindWidget))
	class UImage* MainWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* MainWeaponSlotBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* SubWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* SubWeaponSlotBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* KnifeSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* KnifeSlotBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* GrenadeSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* GrenadeSlotBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* TimeImage;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* InGameTimeText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Time_ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Armor_ProgressBar;

#pragma region InventoryField
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* MainWeaponSizeBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* MainWeaponBox;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SubWeaponSizeBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* SubWeaponBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* KnifeSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* KnifeBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GrenadeNum;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FlashNum;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SmokeNum;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipBox_Head;

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipBox_Top;

#pragma endregion

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* UISwitcher;
};
