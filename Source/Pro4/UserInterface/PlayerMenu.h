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

	// 플레이어 메뉴 기초 설정
	void SetUp();
	virtual bool Initialize();

	/* 플레이어 무기 습득시, ShortCut에서 보여주는 함수 */
	void ActiveWeaponShortcut(uint16 SlotNumber);

	/* 방어구 획득 시, 인벤토리 이미지에서 보여주는 함수 */
	void ActiveArmorImage(bool IsHelmet);

	/* 서버 내 시간을 보여주는 함수 */
	void SetTimeText(uint16 min_, uint16 sec_);

	/* 플레이어 UI에서 이미지를 등록하는 함수 */
	void SetImage(UTexture2D* InTexture);

	/* 위젯 변경 관련 함수 */
	void ChangePlayerWidget();
	void PlayerZoomWidget();
	void ActiveGameOverUI();

	/* 플레이어가 아이템을 획득했을 때, 정보를 저장하는 함수 */
	void AddItemToGrenade(const FString& GrenadeName, uint16 Num);
	void AddItemToWeapon(FString _ImagePath, FString _IconPath, FString _WeaponName);
	void AddItemToInventory(AActor* ItemActor, uint16 Num);

	/* 낮, 밤의 이미지가 저장되는 변수 */
	class UTexture2D* Day;
	class UTexture2D* Night;

private:
	/* 서버 -> 클라이언트들에게 시간을 뿌려주는 함수 */
	UFUNCTION(Client, Reliable)
	void Client_SetTimeText(uint16 min_, uint16 sec_);

	/* 인게임 -> 메인메뉴로 나가는 함수 */
	UFUNCTION()
	void ExitInGame();

	/* 인벤토리 슬롯 클래스 */
	TSubclassOf<class UInventorySlot> InventorySlot;

	/* 플레이어가 무기를 선택할 경우, ShortCut에서 표현되도록 하는 이미지와 경로 */
	UTexture2D* SlotChoose;
	UTexture2D* SlotEmpty;
	FString SlotItemChoosePath = "/Game/UI/Sprites/Player_UI/Item_block/Item_choose";
	FString SlotItemEmptyPath = "/Game/UI/Sprites/Player_UI/Item_block/Item_empty";

	/* Player Default UI에 구성되는 위젯 */
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

	/* 인벤토리 UI에 구성되는 위젯 */
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

	/* 게임오버 UI에 구성되는 위젯 */
#pragma region GameOverField
	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentRanking;
	UPROPERTY(meta = (BindWidget))
	class UImage* MaxRanking;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillPlayerText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillZombieText;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameOverExitButton;
#pragma endregion

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* UISwitcher;
};
