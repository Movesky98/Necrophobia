// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetMenu.h"

/* WidgetMenu가 뷰포트에 보이도록할 때 사용되는 함수 */
void UWidgetMenu::SetUp()
{
	// 뷰포트에 해당 메뉴를 보이도록 함.
	this->AddToViewport();

	FInputModeUIOnly InputModeData;
	this->bIsFocusable = true;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

/* WidgetMenu가 뷰포트에서 없어질 때 실행되는 함수 */
void UWidgetMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;

	// GameOnly모드로 InputMode 변경
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

/* 메뉴 인터페이스를 설정하는 함수 */
void UWidgetMenu::SetMenuInterface(IMenuInterface* OtherMenuInterface)
{
	this->MenuInterface = OtherMenuInterface;
}