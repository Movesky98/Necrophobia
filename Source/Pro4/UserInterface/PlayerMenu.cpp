// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"

void UPlayerMenu::SetUp()
{
	// ����Ʈ�� �ش� �޴��� ���̵��� ��.
	this->AddToViewport();

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}