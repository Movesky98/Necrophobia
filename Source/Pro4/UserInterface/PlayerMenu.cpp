// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"

#include "Components/ProgressBar.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"

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
	UISwitcher->ActiveWidgetIndex = 0;

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

void UPlayerMenu::SetTimeText(uint16 min, uint16 sec)
{
	FString TimeString = FString::FromInt(min);
	TimeString.Append(" : ");
	TimeString.Append(FString::FromInt(sec));
	FText TimeText = FText::FromString(TimeString);

	InGameTimeText->SetText(TimeText);
}

void UPlayerMenu::ChangePlayerWidget()
{
	if (UISwitcher->GetActiveWidgetIndex() == 0)
	{
		UISwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		UISwitcher->SetActiveWidgetIndex(0);
	}
	UE_LOG(Pro4, Warning, TEXT("ActiveWidgetIndex = %d."), UISwitcher->ActiveWidgetIndex);
}