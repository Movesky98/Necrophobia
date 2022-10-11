// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "InventorySlot.h"
#include "../Item/ABaseItem.h"
#include "../Item/AGrenade.h"
#include "../Item/AArmor.h"
#include "../Item/AWeapon.h"
#include "../Pro4PlayerController.h"
#include "../Pro4Character.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/ProgressBar.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"

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
	UInventorySlot* InventoryItem = CreateWidget<UInventorySlot>(GetWorld(), InventorySlot);
	AABaseItem* BaseItem = Cast<AABaseItem>(ItemActor);
	
	switch (BaseItem->ItemType)
	{
	case AABaseItem::BaseItemType::Weapon:
	{
		AAWeapon* Weapon = Cast<AAWeapon>(BaseItem);
		if (!ensure(Weapon != nullptr)) return;
		USkeletalMesh* WeaponStaticMesh = nullptr;
		TArray<USkeletalMeshComponent*> Components;
		Weapon->GetComponents<USkeletalMeshComponent>(Components);

		UE_LOG(Pro4, Warning, TEXT("Weapon Mesh's num : %d"), Components.Num());

		for (int i = 0; i < Components.Num(); i++)
		{
			USkeletalMeshComponent* WeapoonStaticMeshComponents = Components[i];
			WeaponStaticMesh = WeapoonStaticMeshComponents->SkeletalMesh;
		}

		APro4Character* MyPawn = Cast<APro4Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
		MyPawn->SetPlayerWeapon(WeaponStaticMesh);

		InventoryItem->SetUp(Weapon->ItemName, Weapon->ItemNum, Weapon->GetIconPath());
		UE_LOG(Pro4, Warning, TEXT("Icon Path : %s"), *Weapon->GetIconPath());
		InventoryBox->AddChildToWrapBox(InventoryItem);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InventoryItem->GetItemName());
	}
		break;
	case AABaseItem::BaseItemType::Armor:
	{
		AAArmor* Armor = Cast<AAArmor>(BaseItem);
		InventoryItem->SetUp(Armor->ItemName, Armor->ItemNum, "Hello");
		InventoryBox->AddChildToWrapBox(InventoryItem);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InventoryItem->GetItemName());
	}
		break;
	case AABaseItem::BaseItemType::Grenade:
	{
		AAGrenade* Grenade = Cast<AAGrenade>(BaseItem);
		InventoryItem->SetUp(Grenade->ItemName, Grenade->ItemNum, "Hello");
		InventoryBox->AddChildToWrapBox(InventoryItem);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InventoryItem->GetItemName());
	}
		break;
	case AABaseItem::BaseItemType::Recovery:
		// TO DO : Implement Item of Recovery
		break;
	case AABaseItem::BaseItemType::Ammo:
		// TO DO : Implement Item of Ammo
		break;
	case AABaseItem::BaseItemType::Parts:
		// TO DO : Implement Item of Parts
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