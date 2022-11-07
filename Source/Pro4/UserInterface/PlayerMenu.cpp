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

#include "UObject/ConstructorHelpers.h"

#include "Components/ProgressBar.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

UPlayerMenu::UPlayerMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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

bool UPlayerMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InGameTimeText != nullptr)) return false;
	if (!ensure(Time_ProgressBar != nullptr)) return false;
	if (!ensure(HP_ProgressBar != nullptr)) return false;
	if (!ensure(Armor_ProgressBar != nullptr)) return false;

	return true;
}

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
}

/* 인게임 내 플레이 시간을 표기하는 함수 */
void UPlayerMenu::SetTimeText(uint16 min_, uint16 sec_)
{
	Client_SetTimeText(min_, sec_);
}

void UPlayerMenu::Client_SetTimeText_Implementation(uint16 min_, uint16 sec_)
{
	FString TimeString = FString::FromInt(min_);
	TimeString.Append(" : ");
	TimeString.Append(FString::FromInt(sec_));
	FText TimeText = FText::FromString(TimeString);

	InGameTimeText->SetText(TimeText);
}

void UPlayerMenu::ChangePlayerWidget()
{

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

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
		UISwitcher->SetActiveWidgetIndex(0);

		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
	UE_LOG(Pro4, Warning, TEXT("ActiveWidgetIndex = %d."), UISwitcher->ActiveWidgetIndex);
}

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

void UPlayerMenu::AddItemToInventory(AActor* ItemActor, uint16 Num)
{
	AABaseItem* BaseItem = Cast<AABaseItem>(ItemActor);
	APro4Character* MyPawn = Cast<APro4Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

	switch (BaseItem->ItemType)
	{
	case AABaseItem::BaseItemType::Weapon:
	{
		AAWeapon* NewWeapon = Cast<AAWeapon>(BaseItem);
		if (!ensure(NewWeapon != nullptr)) return;

		MyPawn->SetPlayerWeapon(NewWeapon);

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
		AAArmor* Armor = Cast<AAArmor>(BaseItem);
		if (!ensure(Armor != nullptr)) return;

		MyPawn->SetPlayerArmor(Armor);
		/*if (Armor->GetItemName() == "Helmet")
		{
			ActiveArmorImage(true);
		}
		else
		{

		}*/

		(Armor->GetItemName() == "Helmet") ? ActiveArmorImage(true) : ActiveArmorImage(false);
		

	}
		break;
	case AABaseItem::BaseItemType::Grenade:
	{
		AAGrenade* Grenade = Cast<AAGrenade>(BaseItem);
		if (!ensure(Grenade != nullptr)) return;

		MyPawn->AddPlayerGrenade(Grenade);
	}
		break;
	case AABaseItem::BaseItemType::Recovery:
	{
		// TO DO : Implement Item of Recovery
		ARecovery* Recovery = Cast<ARecovery>(BaseItem);

		UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
		InventoryItem->SetUp("Recovery", Recovery->GetItemName(), Recovery->GetItemNum(), Recovery->GetIconPath());
		InventoryBox->AddChildToWrapBox(InventoryItem);
	}
		break;
	case AABaseItem::BaseItemType::Ammo:
	{
		// TO DO : Implement Item of Ammo
		AAmmo* Ammo = Cast<AAmmo>(BaseItem);

		UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
		InventoryItem->SetUp("Ammo", Ammo->GetItemName(), Ammo->GetItemNum(), Ammo->GetIconPath());
		InventoryBox->AddChildToWrapBox(InventoryItem);

		MyPawn->SetPlayerRound(Ammo);
	}
		break;
	case AABaseItem::BaseItemType::Vaccine:
	{
		// TO DO : Implement Item of Vaccine
		AVaccine* Vaccine = Cast<AVaccine>(BaseItem);
		MyPawn->SetIsPossibleEscapeOnServer(true);

		UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
		InventoryItem->SetUp("Vaccine", Vaccine->GetItemName(), Vaccine->GetItemNum(), Vaccine->GetIconPath());
		InventoryBox->AddChildToWrapBox(InventoryItem);
	}
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Add Item To Inventory ERROR"));
		break;
	}
}

void UPlayerMenu::SetImage(UTexture2D* InTexture)
{
	TimeImage->SetBrushFromTexture(InTexture);
}

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

void UPlayerMenu::AddItemToWeapon(FString _ImagePath, FString _IconPath, FString _WeaponName)
{
	// Image를 그림
	UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_ImagePath), NULL, LOAD_None, NULL);
	UTexture2D* ItemIcon = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_IconPath), NULL, LOAD_None, NULL);

	if (_WeaponName == "Pistol")
	{
		SubWeaponBox->SetBrushFromTexture(ItemImage);
		SubWeaponSlot->SetBrushFromTexture(ItemIcon);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *SubWeaponBox->Brush.GetResourceName().ToString());
	}
	else if (_WeaponName == "Knife")
	{
		KnifeBox->SetBrushFromTexture(ItemImage);
		KnifeSlot->SetBrushFromTexture(ItemIcon);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *KnifeBox->Brush.GetResourceName().ToString());
	}
	else
	{
		MainWeaponBox->SetBrushFromTexture(ItemImage);
		MainWeaponSlot->SetBrushFromTexture(ItemIcon);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *MainWeaponBox->Brush.GetResourceName().ToString());
	}
}

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