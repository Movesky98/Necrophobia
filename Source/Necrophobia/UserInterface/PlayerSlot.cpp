// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSlot.h"

#include "Components/TextBlock.h"
#include "Components/CheckBox.h"

UPlayerSlot::UPlayerSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), PlayerName(FName())
{

}

void UPlayerSlot::InitializePlayerInfo(const FName& Name, bool bIsNewReady)
{
	if (Name.IsValid())
	{
		PlayerName = Name;

		bIsReady = bIsNewReady;
		UpdateReadyCheckBox(bIsReady);
	}
	else
	{
		// Log Error...
	}
}

void UPlayerSlot::UpdateReadyCheckBox(bool bIsNewReady)
{
	if (bIsNewReady)
		IsReadyCheckBox->SetCheckedState(ECheckBoxState::Checked);
	else
		IsReadyCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
}

FString UPlayerSlot::GetPlayerName()
{
	return PlayerName.ToString();
}

void UPlayerSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerNameText->SetText(FText::FromName(PlayerName));
}
