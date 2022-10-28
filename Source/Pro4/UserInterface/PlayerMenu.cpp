// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "InventorySlot.h"

#include "../Item/ABaseItem.h"
#include "../Item/AGrenade.h"
#include "../Item/AArmor.h"
#include "../Item/AWeapon.h"
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
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetItemName());
			
			if (MainWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				MainWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if (NewWeapon->GetItemName() == "SR")
		{
			MainWeaponSizeBox->SetWidthOverride(475);
			MainWeaponSizeBox->SetHeightOverride(112);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetItemName());

			if (MainWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				MainWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if(NewWeapon->GetItemName() == "Pistol")
		{
			SubWeaponSizeBox->SetWidthOverride(149);
			SubWeaponSizeBox->SetHeightOverride(145);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetItemName());

			if (SubWeaponBox->GetVisibility() == ESlateVisibility::Hidden)
			{
				SubWeaponBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			MainWeaponSizeBox->SetWidthOverride(475);
			MainWeaponSizeBox->SetHeightOverride(136);
			AddItemToWeapon(NewWeapon->GetBoxImagePath(), NewWeapon->GetItemName());

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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InventoryItem->GetItemName());
	}
		break;
	case AABaseItem::BaseItemType::Ammo:
		// TO DO : Implement Item of Ammo
		break;
	case AABaseItem::BaseItemType::Vaccine:
	{
		// TO DO : Implement Item of Vaccine
		AVaccine* Vaccine = Cast<AVaccine>(BaseItem);
		MyPawn->SetIsPossibleEscape(true);

		UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
		InventoryItem->SetUp("Vaccine", Vaccine->GetItemName(), Vaccine->GetItemNum(), Vaccine->GetIconPath());
		InventoryBox->AddChildToWrapBox(InventoryItem);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InventoryItem->GetItemName());
		
	}
		break;
	default:
		UE_LOG(Pro4, Warning, TEXT("Add item to Inventory ERROR"));
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

void UPlayerMenu::AddItemToWeapon(FString _IconPath, FString _WeaponName)
{
	if (_WeaponName == "Pistol")
	{
		// Image를 그림
		UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_IconPath), NULL, LOAD_None, NULL);

		SubWeaponBox->SetBrushFromTexture(ItemImage);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *SubWeaponBox->Brush.GetResourceName().ToString());
	}
	else if (_WeaponName == "Knife")
	{
		// Image를 그림
		UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_IconPath), NULL, LOAD_None, NULL);

		KnifeBox->SetBrushFromTexture(ItemImage);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *KnifeBox->Brush.GetResourceName().ToString());
	}
	else
	{
		// Image를 그림
		UTexture2D* ItemImage = LoadObject<UTexture2D>(NULL, (TEXT("%s"), *_IconPath), NULL, LOAD_None, NULL);

		MainWeaponBox->SetBrushFromTexture(ItemImage);

		UE_LOG(Pro4, Warning, TEXT("Image Object Name : %s"), *MainWeaponBox->Brush.GetResourceName().ToString());
	}
}

void UPlayerMenu::SetPlayerHP(float CurHP, float MaxHP)
{
	HP_ProgressBar->Percent = CurHP / MaxHP;
}

void UPlayerMenu::SetPlayerAP(float CurAP, float MaxAP)
{
	Armor_ProgressBar->Percent = CurAP / MaxAP;
}