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
	// Join (As Client) with Address
	if (MenuInterface != nullptr)
	{
		if (!ensure(NicknameField != nullptr)) return;

		const FString& Address = NicknameField->GetText().ToString();
		// 여기서 세션 개수만큼 랜덤하게 생성하여 참여하는 알고리즘 필요
		// 환경설정을 로드해오는 함수므로 Host할 때 하는게 맞는 듯.
		MenuInterface->Join(0);
	}

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
