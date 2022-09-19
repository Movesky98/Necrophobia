// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerController.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(GameStartButton != nullptr)) return false;
	if (!ensure(PreferenceButton != nullptr)) return false;
	if (!ensure(GameRecordButton != nullptr)) return false;
	if (!ensure(ExitButton != nullptr)) return false;

	GameStartButton->OnClicked.AddDynamic(this, &UMainMenu::GameStart);
	PreferenceButton->OnClicked.AddDynamic(this, &UMainMenu::LoadPreference);
	GameRecordButton->OnClicked.AddDynamic(this, &UMainMenu::LoadGameRecord);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::GameStart()
{
	// Host
	if (MenuInterface != nullptr)
	{
		MenuInterface->FindSessionList();
	}
}

void UMainMenu::LoadPreference()
{
	

}

void UMainMenu::LoadGameRecord()
{

}

void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}
