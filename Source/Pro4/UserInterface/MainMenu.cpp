// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerController.h"

/* UI 위젯 가져오는데에 문제가 없는지 확인하는 함수 */
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(GameStartButton != nullptr)) return false;
	if (!ensure(PreferenceButton != nullptr)) return false;
	if (!ensure(GameRecordButton != nullptr)) return false;
	if (!ensure(ExitButton != nullptr)) return false;

	// 각 버튼이 눌렸을 경우, 실행되는 함수들을 등록해놓음.
	GameStartButton->OnClicked.AddDynamic(this, &UMainMenu::GameStart);
	PreferenceButton->OnClicked.AddDynamic(this, &UMainMenu::LoadPreference);
	GameRecordButton->OnClicked.AddDynamic(this, &UMainMenu::LoadGameRecord);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

/* 게임 시작 버튼을 눌렀을 경우 실행되는 함수 */
void UMainMenu::GameStart()
{
	// 세션 리스트가 있는지 (온라인 세션) 검색
	if (MenuInterface != nullptr)
	{
		MenuInterface->FindSessionList();
	}
}

/* 환경설정 버튼을 눌렀을 경우 실행되는 함수 */
void UMainMenu::LoadPreference()
{
	

}

/* 게임 기록실 버튼을 눌렀을 경우 실행되는 함수 */
void UMainMenu::LoadGameRecord()
{

}

/* 게임 나가기 버튼을 눌렀을 경우 실행되는 함수 */
void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}
