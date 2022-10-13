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

	virtual bool Initialize();

	void SetTimeText(uint16 min_, uint16 sec_);
	void SetImage(UTexture2D* InTexture);

	void ChangePlayerWidget();

	void AddItemToWeapon(FString _IconPath, FString _WeaponName);
	void AddItemToInventory(AActor* ItemActor, uint16 Num);

	class UTexture2D* Day;
	class UTexture2D* Night;

private:

	UFUNCTION(Client, Reliable)
	void Client_SetTimeText(uint16 min_, uint16 sec_);

	TSubclassOf<class UInventorySlot> InventorySlot;

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
#pragma endregion

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* UISwitcher;
};
