// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerController.h"

/* UI ���� �������µ��� ������ ������ Ȯ���ϴ� �Լ� */
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(GameStartButton != nullptr)) return false;
	if (!ensure(PreferenceButton != nullptr)) return false;
	if (!ensure(GameRecordButton != nullptr)) return false;
	if (!ensure(ExitButton != nullptr)) return false;

	// �� ��ư�� ������ ���, ����Ǵ� �Լ����� ����س���.
	GameStartButton->OnClicked.AddDynamic(this, &UMainMenu::GameStart);
	PreferenceButton->OnClicked.AddDynamic(this, &UMainMenu::LoadPreference);
	GameRecordButton->OnClicked.AddDynamic(this, &UMainMenu::LoadGameRecord);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

/* ���� ���� ��ư�� ������ ��� ����Ǵ� �Լ� */
void UMainMenu::GameStart()
{
	// ���� ����Ʈ�� �ִ��� (�¶��� ����) �˻�
	if (MenuInterface != nullptr)
	{
		MenuInterface->FindSessionList();
	}
}

/* ȯ�漳�� ��ư�� ������ ��� ����Ǵ� �Լ� */
void UMainMenu::LoadPreference()
{
	

}

/* ���� ��Ͻ� ��ư�� ������ ��� ����Ǵ� �Լ� */
void UMainMenu::LoadGameRecord()
{

}

/* ���� ������ ��ư�� ������ ��� ����Ǵ� �Լ� */
void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}
