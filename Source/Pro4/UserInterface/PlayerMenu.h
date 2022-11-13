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

	// �÷��̾� �޴� ���� ����
	void SetUp();
	virtual bool Initialize();

	void SetFlashBangImage();
	void RecoverPlayerFlashbang();

	/* �������� ȹ���� ���, �κ��丮�� ������ ���� ������Ʈ���ִ� �Լ� */
	bool UpdateInventoryBox(FString ItemName, uint16 Num);

	/* �κ��丮�� ����, �����۵��� ������ ������Ʈ ���ִ� �Լ� */
	void UpdatePlayerWeaponAmmo(uint16 MainWeaponRounds, uint16 SubWeaponRounds);

	/* �÷��̾� ���� �����, ShortCut���� �����ִ� �Լ� */
	void ActiveWeaponShortcut(uint16 SlotNumber);

	/* �� ȹ�� ��, �κ��丮 �̹������� �����ִ� �Լ� */
	void ActiveArmorImage(bool IsHelmet);

	/* ���� �� �ð��� �����ִ� �Լ� */
	void SetTimeText(uint16 min_, uint16 sec_);

	/* �÷��̾� UI���� �̹����� ����ϴ� �Լ� */
	void SetImage(UTexture2D* InTexture);

	/* ���� ���� ���� �Լ� */
	void ChangePlayerWidget();
	void PlayerZoomWidget();
	void ActiveGameOverUI(uint16 PlayerKill, uint16 ZombieKill, uint16 PlayerRanking, uint16 TotalPlayer);
	
	/* ���� ���� UI����, ��ŷ�� ���� �̹����� �����ִ� �Լ� */
	void SetRankingUI(uint16 PlayerRanking, uint16 TotalPlayer);

	/* �÷��̾� �޴� (�⺻�޴�) ���� ���� �Ѿ��� ������Ʈ�ϴ� �Լ� */
	void UpdatePlayerRounds(uint16 CurrentRound, uint16 TotalRound);

	/* �÷��̾ �������� ȹ������ ��, ������ �����ϴ� �Լ� */
	void AddItemToGrenade(const FString& GrenadeName, uint16 Num);
	void AddItemToWeapon(FString _ImagePath, FString _IconPath, FString _WeaponName);
	void AddItemToInventory(AActor* ItemActor, uint16 Num);

	/* ��, ���� �̹����� ����Ǵ� ���� */
	class UTexture2D* Day;
	class UTexture2D* Night;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float PlayerFlashDegree;

private:
	/* ���� -> Ŭ���̾�Ʈ�鿡�� �ð��� �ѷ��ִ� �Լ� */
	UFUNCTION(Client, Reliable)
	void Client_SetTimeText(uint16 min_, uint16 sec_);

	UFUNCTION(Server, Reliable)
	void DestroyItem(AActor* DestroyActor);

	/* �ΰ��� -> ���θ޴��� ������ �Լ� */
	UFUNCTION()
	void ExitInGame();

	FTimerHandle FlashBangTimer; 

	/* �κ��丮 ���� Ŭ���� */
	TSubclassOf<class UInventorySlot> InventorySlot;

	/* �÷��̾ ���⸦ ������ ���, ShortCut���� ǥ���ǵ��� �ϴ� �̹����� ��� */
	UTexture2D* SlotChoose;
	UTexture2D* SlotEmpty;
	FString SlotItemChoosePath = "/Game/UI/Sprites/Player_UI/Item_block/Item_choose";
	FString SlotItemEmptyPath = "/Game/UI/Sprites/Player_UI/Item_block/Item_empty";
	
	/* Player Default UI�� �����Ǵ� ���� */
#pragma region PlayerDefaultUI
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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoundsText;
#pragma endregion

	/* �κ��丮 UI�� �����Ǵ� ���� */
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

	/* ���ӿ��� UI�� �����Ǵ� ���� */
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

	UPROPERTY(meta = (BindWidget))
	class UImage* FlashBangImage;
};
