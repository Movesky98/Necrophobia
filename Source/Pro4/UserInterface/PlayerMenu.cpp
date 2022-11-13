// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "InventorySlot.h"

#include "../Item/ABaseItem.h"
#include "../Item/AGrenade.h"
#include "../Item/AArmor.h"
#include "../Item/AWeapon.h"
#include "../Item/Ammo.h"
#include "../Item/Vaccine.h"
#include "../Item/Recovery.h"

#include "../Pro4PlayerController.h"
#include "../Pro4Character.h"
#include "../NecrophobiaGameInstance.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/ProgressBar.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"

UPlayerMenu::UPlayerMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// InventorySlot 클래스를 가져옴
	ConstructorHelpers::FClassFinder<UInventorySlot> InventorySlotClass(TEXT("/Game/UI/Player/WBP_InventorySlot"));
	if (!ensure(InventorySlotClass.Class != nullptr)) return;

	InventorySlot = InventorySlotClass.Class;

	ConstructorHelpers::FObjectFinder<UTexture2D> DayObject(TEXT("/Game/UI/Sprites/Player_UI/Time_change/Time_Daytime"));
	if (!ensure(DayObject.Object != nullptr)) return;

	Day = DayObject.Object;

	ConstructorHelpers::FObjectFinder<UTexture2D> NightObject(TEXT("/Game/UI/Sprites/Player_UI/Time_change/Time_Night"));
	if (!ensure(NightObject.Object != nullptr)) return;

	Night = NightObject.Object;

	SlotChoose = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *SlotItemChoosePath), NULL, LOAD_None, NULL);
	SlotEmpty = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *SlotItemEmptyPath), NULL, LOAD_None, NULL);
}

/* UI 위젯 가져오는데에 문제가 없는지 확인하는 함수 */
bool UPlayerMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InGameTimeText != nullptr)) return false;
	if (!ensure(Time_ProgressBar != nullptr)) return false;
	if (!ensure(HP_ProgressBar != nullptr)) return false;
	if (!ensure(Armor_ProgressBar != nullptr)) return false;
	if (!ensure(GameOverExitButton != nullptr)) return false;
	
	GameOverExitButton->OnClicked.AddDynamic(this, &UPlayerMenu::ExitInGame);
	
	return true;
}

/* 플레이어 UI 초기 세팅 */
void UPlayerMenu::SetUp()
{
	// 뷰포트에 해당 메뉴를 보이도록 함.
	this->AddToViewport(); 
	UISwitcher->SetActiveWidgetIndex(0);

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);

	MainWeaponBox->SetVisibility(ESlateVisibility::Hidden);
	SubWeaponBox->SetVisibility(ESlateVisibility::Hidden);
	KnifeBox->SetVisibility(ESlateVisibility::Hidden);

	RoundsText->SetText(FText::FromString(FString("0 / 0")));

	PlayerFlashDegree = 0;
}

/* 인게임 내 플레이 시간을 표기하는 함수 */
void UPlayerMenu::SetTimeText(uint16 min_, uint16 sec_)
{
	Client_SetTimeText(min_, sec_);
}

/* 서버 -> 클라이언트들에게 시간을 뿌려줌 */
void UPlayerMenu::Client_SetTimeText_Implementation(uint16 min_, uint16 sec_)
{
	FString TimeString = FString::FromInt(min_);
	TimeString.Append(" : ");
	TimeString.Append(FString::FromInt(sec_));
	FText TimeText = FText::FromString(TimeString);

	InGameTimeText->SetText(TimeText);
}

/* PlayeDefaultUI <-> Inventory UI랑 변경하기 위한 함수 */
void UPlayerMenu::ChangePlayerWidget()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	/* PlayerDefault UI -> Inventory UI */
	if (UISwitcher->GetActiveWidgetIndex() == 0)
	{
		UISwitcher->SetActiveWidgetIndex(1);

		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		/* Inventory UI -> PlayerDefault UI*/
		UISwitcher->SetActiveWidgetIndex(0);

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
	UE_LOG(Pro4, Warning, TEXT("ActiveWidgetIndex = %d."), UISwitcher->ActiveWidgetIndex);
}

/* 플레이어가 줌 했을 경우 실행되는 함수 */
void UPlayerMenu::PlayerZoomWidget()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	if (UISwitcher->GetActiveWidgetIndex() != 2)
	{
		UISwitcher->SetActiveWidgetIndex(2);

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
	else
	{
		UISwitcher->SetActiveWidgetIndex(0);

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
}

/* 게임 오버 UI를 활성화 하는 함수 */
void UPlayerMenu::ActiveGameOverUI(uint16 PlayerKill, uint16 ZombieKill, uint16 PlayerRanking, uint16 TotalPlayer)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FString PlayerKillNum = FString::FromInt(PlayerKill);
	FString ZombieKillNum = FString::FromInt(ZombieKill);
	
	KillPlayerText->SetText(FText::FromString(PlayerKillNum));
	KillZombieText->SetText(FText::FromString(ZombieKillNum));

	SetRankingUI(PlayerRanking, TotalPlayer);

	UISwitcher->SetActiveWidgetIndex(3);

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

/* 게임이 종료하기 전, 플레이어의 랭킹을 띄워주는 함수 */
void UPlayerMenu::SetRankingUI(uint16 PlayerRanking, uint16 TotalPlayer)
{
	FString CurrentRankingPath;
	FString TotalRankingPath;

	// 이미지 랭킹에 따른 이미지 경로 연결
	switch (PlayerRanking)
	{
	case 1:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_1";
		break;
	case 2:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_2";
		break;
	case 3:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_3";
		break;
	case 4:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_4";
		break;
	case 5:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_5";
		break;
	case 6:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_6";
		break;
	case 7:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_7";
		break;
	case 8:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_8";
		break;
	case 9:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_9";
		break;
	default:
		CurrentRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_0";
		break;
	}

	// 이미지 랭킹에 따른 이미지 경로 연결
	switch (TotalPlayer)
	{
	case 1:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_1";
		break;
	case 2:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_2";
		break;
	case 3:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_3";
		break;
	case 4:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_4";
		break;
	case 5:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_5";
		break;
	case 6:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_6";
		break;
	case 7:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_7";
		break;
	case 8:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_8";
		break;
	case 9:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_9";
		break;
	default:
		TotalRankingPath = "/Game/UI/Sprites/Gameover_UI/GameoverUI_0";
		break;
	}

	UTexture2D* CurrentRankingImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *CurrentRankingPath), NULL, LOAD_None, NULL);
	UTexture2D* TotalRankingImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *TotalRankingPath), NULL, LOAD_None, NULL);

	CurrentRanking->SetBrushFromTexture(CurrentRankingImage);
	MaxRanking->SetBrushFromTexture(TotalRankingImage);
}

/* 플레이어가 Item을 획득했을 때, 실행되는 함수 */
void UPlayerMenu::AddItemToInventory(AActor* ItemActor, uint16 Num)
{
	AABaseItem* BaseItem = Cast<AABaseItem>(ItemActor);
	APro4Character* MyPawn = Cast<APro4Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
	bool isUpdated;

	switch (BaseItem->ItemType)
	{
	case AABaseItem::BaseItemType::Weapon:
	{
		// 무기 획득
		AAWeapon* NewWeapon = Cast<AAWeapon>(BaseItem);
		if (!ensure(NewWeapon != nullptr)) return;

		MyPawn->SetPlayerWeapon(NewWeapon);

		// 인벤토리 UI에 무기가 보이도록 설정
		if (NewWeapon->GetItemName() == "AR")
		{
			MainWeaponSizeBox->SetWidthOverride(475);
			MainWeaponSizeBox->SetHeightOverride(136);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetIconPath(), NewWeapon->GetItemName());
			
			if (MainWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				MainWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if (NewWeapon->GetItemName() == "SR")
		{
			MainWeaponSizeBox->SetWidthOverride(475);
			MainWeaponSizeBox->SetHeightOverride(112);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetIconPath(), NewWeapon->GetItemName());

			if (MainWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				MainWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if(NewWeapon->GetItemName() == "Pistol")
		{
			SubWeaponSizeBox->SetWidthOverride(149);
			SubWeaponSizeBox->SetHeightOverride(145);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetIconPath(), NewWeapon->GetItemName());

			if (SubWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				SubWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			MainWeaponSizeBox->SetWidthOverride(475);
			MainWeaponSizeBox->SetHeightOverride(136);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetIconPath(), NewWeapon->GetItemName());

			if (KnifeBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				KnifeBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
		break;
	case AABaseItem::BaseItemType::Armor:
	{
		// 방어구 획득
		AAArmor* Armor = Cast<AAArmor>(BaseItem);
		if (!ensure(Armor != nullptr)) return;

		MyPawn->SetPlayerArmor(Armor);

		(Armor->GetItemName() == "Helmet") ? ActiveArmorImage(true) : ActiveArmorImage(false);
	}
		break;
	case AABaseItem::BaseItemType::Grenade:
	{
		// 투척 무기 획득
		AAGrenade* Grenade = Cast<AAGrenade>(BaseItem);
		if (!ensure(Grenade != nullptr)) return;

		MyPawn->AddPlayerGrenade(Grenade);
	}
		break;
	case AABaseItem::BaseItemType::Recovery:
	{
		// 잠식 치료제 획득
		ARecovery* Recovery = Cast<ARecovery>(BaseItem);
		
		isUpdated = UpdateInventoryBox(Recovery->GetItemName(), Recovery->GetItemNum());

		if (!isUpdated)
		{
			UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
			InventoryItem->SetUp("Recovery", Recovery->GetItemName(), Recovery->GetItemNum(), Recovery->GetIconPath());
			InventoryBox->AddChildToWrapBox(InventoryItem);
		}

		DestroyItem(Recovery);
	}
		break;
	case AABaseItem::BaseItemType::Ammo:
	{
		// 탄약 획득
		AAmmo* Ammo = Cast<AAmmo>(BaseItem);

		isUpdated = UpdateInventoryBox(Ammo->GetItemName(), Ammo->GetItemNum());

		if (!isUpdated)
		{
			UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
			InventoryItem->SetUp("Ammo", Ammo->GetItemName(), Ammo->GetItemNum(), Ammo->GetIconPath());

			InventoryBox->AddChildToWrapBox(InventoryItem);
		}

		MyPawn->SetPlayerRound(Ammo);
	}
		break;
	case AABaseItem::BaseItemType::Vaccine:
	{
		// 백신 획득
		AVaccine* Vaccine = Cast<AVaccine>(BaseItem);
		MyPawn->SetIsPossibleEscapeOnServer(true);

		isUpdated = UpdateInventoryBox(Vaccine->GetItemName(), Vaccine->GetItemNum());

		if (!isUpdated)
		{
			UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
			InventoryItem->SetUp("Vaccine", Vaccine->GetItemName(), Vaccine->GetItemNum(), Vaccine->GetIconPath());
			InventoryBox->AddChildToWrapBox(InventoryItem);
		}

		DestroyItem(Vaccine);
	}
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Add Item To Inventory ERROR"));
		break;
	}
}

/* 낮/밤에 따른 이미지 활성화 */
void UPlayerMenu::SetImage(UTexture2D* InTexture)
{
	TimeImage->SetBrushFromTexture(InTexture);
}

/* 인벤토리 UI에 투척 무기 수를 보여주는 함수 */
void UPlayerMenu::AddItemToGrenade(const FString& GrenadeName, uint16 Num)
{
	FString GrenadeNumber = FString::FromInt(Num);

	if (!GrenadeName.Compare("Grenade"))
	{
		GrenadeNum->SetText(FText::FromString(GrenadeNumber));
	}
	else if (!GrenadeName.Compare("Smoke"))
	{
		SmokeNum->SetText(FText::FromString(GrenadeNumber));
	}
	else if (!GrenadeName.Compare("Flash"))
	{
		FlashNum->SetText(FText::FromString(GrenadeNumber));
	}
}

/* 플레이어가 무기를 획득할 경우, 해당 무기의 이미지를 인벤토리 UI에서 보여주는 함수 */
void UPlayerMenu::AddItemToWeapon(FString _ImagePath, FString _IconPath, FString _WeaponName)
{
	// Image를 그림
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_ImagePath), NULL, LOAD_None, NULL);
	UTexture2D* ItemIcon = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_IconPath), NULL, LOAD_None, NULL);

	if (_WeaponName == "Pistol")
	{
		SubWeaponBox->SetBrushFromTexture(ItemImage);
		SubWeaponSlot->SetBrushFromTexture(ItemIcon);
	}
	else if (_WeaponName == "Knife")
	{
		KnifeBox->SetBrushFromTexture(ItemImage);
		KnifeSlot->SetBrushFromTexture(ItemIcon);
	}
	else
	{
		MainWeaponBox->SetBrushFromTexture(ItemImage);
		MainWeaponSlot->SetBrushFromTexture(ItemIcon);
	}
}

/* 플레이어가 무기를 착용할 경우, 표시되는 Shotcut */
void UPlayerMenu::ActiveWeaponShortcut(uint16 SlotNumber)
{
	MainWeaponSlotBox->SetBrushFromTexture(SlotEmpty);
	SubWeaponSlotBox->SetBrushFromTexture(SlotEmpty);
	KnifeSlotBox->SetBrushFromTexture(SlotEmpty);
	GrenadeSlotBox->SetBrushFromTexture(SlotEmpty);
	switch (SlotNumber)
	{
	case 1:
		MainWeaponSlotBox->SetBrushFromTexture(SlotChoose);
		break;
	case 2:
		SubWeaponSlotBox->SetBrushFromTexture(SlotChoose);
		break;
	case 3:
		KnifeSlotBox->SetBrushFromTexture(SlotChoose);
		break;
	case 4:
		GrenadeSlotBox->SetBrushFromTexture(SlotChoose);
		break;
	default:
		break;
	}
}

/* 방어구를 획득할 경우, 인벤토리에서 보이도록 하는 함수 */
void UPlayerMenu::ActiveArmorImage(bool IsHelmet)
{
	if (IsHelmet)
	{
		FString Path = "/Game/UI/Sprites/Weapon_Icon/Helmet_Icon_500x500";
		UTexture2D* HelmetImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);
		EquipBox_Head->SetBrushFromTexture(HelmetImage);
		EquipBox_Head->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FString Path = "/Game/UI/Sprites/Weapon_Icon/Vest_Icon_500x500";
		UTexture2D* VestImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *Path), NULL, LOAD_None, NULL);
		EquipBox_Top->SetBrushFromTexture(VestImage);
		EquipBox_Top->SetVisibility(ESlateVisibility::Visible);
	}
}

/* 플레이어의 탄알 수를 업데이트 하는 함수 */
void UPlayerMenu::UpdatePlayerRounds(uint16 CurrentRound, uint16 TotalRound)
{
	FString RoundText = FString::FromInt(CurrentRound) + " / " + FString::FromInt(TotalRound);

	RoundsText->SetText(FText::FromString(RoundText));
}

/* 인벤토리 내의 박스를 업데이트하는 함수 */
bool UPlayerMenu::UpdateInventoryBox(FString ItemName, uint16 Num)
{
	TArray<UWidget*> PlayerInventory = InventoryBox->GetAllChildren();

	// 인벤토리 안에 아이템이 존재한다면
	if (PlayerInventory.Num())
	{
		for (UWidget* ItemWidget : PlayerInventory)
		{
			UInventorySlot* ItemSlot = Cast<UInventorySlot>(ItemWidget);

			if (ItemSlot->GetItemName() == ItemName)
			{
				ItemSlot->SetItemNum(ItemSlot->GetItemNum() + Num);
				ItemSlot->UpdateSlotCount();
				return true;
			}
		}
	}

	return false;
}

/* 인벤토리 UI 내의 아이템 중, 무기 탄약과 관련된 아이템일 경우 수를 최신화하는 함수 */
void UPlayerMenu::UpdatePlayerWeaponAmmo(uint16 MainWeaponRounds, uint16 SubWeaponRounds)
{
	TArray<UWidget*> PlayerInventory = InventoryBox->GetAllChildren();

	// 인벤토리 안에 아이템이 존재한다면
	if (PlayerInventory.Num())
	{
		for (UWidget* ItemWidget : PlayerInventory)
		{
			UInventorySlot* ItemSlot = Cast<UInventorySlot>(ItemWidget);

			if (ItemSlot->GetItemType() == "Ammo")
			{
				if (ItemSlot->GetItemName() == "MainWeaponAmmo")
				{
					ItemSlot->SetItemNum(MainWeaponRounds);
				}
				else
				{
					ItemSlot->SetItemNum(SubWeaponRounds);
				}
			}

			ItemSlot->UpdateSlotCount();
		}
	}
}

/* 인게임 -> 메인메뉴로 나갈때 실행되는 함수 */
void UPlayerMenu::ExitInGame()
{
	UNecrophobiaGameInstance* NecGameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());

	NecGameInstance->LoadMainMenu();
}

/* 플레이어가 섬광탄에 맞았을 때 실행되는 함수 */
void UPlayerMenu::SetFlashBangImage()
{
	PlayerFlashDegree = 1.0f;

	GetWorld()->GetTimerManager().SetTimer(FlashBangTimer, this, &UPlayerMenu::RecoverPlayerFlashbang, 0.01f, true, 3.0f);
}

/* 플레이어가 섬광탄에 맞고난 후, 회복하는 함수 */
void UPlayerMenu::RecoverPlayerFlashbang()
{
	PlayerFlashDegree -= 0.005f;

	if (PlayerFlashDegree <= 0)
	{
		PlayerFlashDegree = 0;

		GetWorld()->GetTimerManager().ClearTimer(FlashBangTimer);
	}
}

/* 아이템을 획득했을 때, 실행되는 함수. 월드 내에 있는 아이템을 삭제함. */
void UPlayerMenu::DestroyItem_Implementation(AActor* DestroyActor)
{
	DestroyActor->Destroy();
}