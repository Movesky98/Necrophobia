// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4PlayerController.h"
#include "PlayerDefaultWidget.h"

APro4PlayerController::APro4PlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerDefaultWidget> BP_PlayerUI(TEXT("/Game/UI/PlayerUI.PlayerUI_C"));
	if (BP_PlayerUI.Succeeded())
	{
		PlayerWidgetClass = BP_PlayerUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UPlayerDefaultWidget> LobbyUI(TEXT("/Game/UI/LobbyUI/LobbyUIWidget.LobbyUIWidget_C"));
	if(LobbyUI.Succeeded())
	{
		LobbyWidgetClass = LobbyUI.Class;
	}
}

void APro4PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		// SetInputMode에 FInputModeGameAndUI를 넣어주면
		// 게임과 UI 모두 입력 가능한 상태가 된다.
		SetInputMode(FInputModeGameOnly());
	}
}

/*
* UI를 변경할 때, 실행되는 함수
*/
void APro4PlayerController::ChangeMenuWidget(TSubclassOf<class UPlayerDefaultWidget> NewWidgetClass)
{
	// 먼저 CurrentWidget이 비어있는지 확인
	if (CurrentWidget != nullptr)
	{
		// 비어있지 않다면 화면에서 UI를 제거한 후 CurrentWidget를 비워줌
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	// 이후 매개 변수로 받은 NewWidgetClass가 유효한지 검사
	if (NewWidgetClass != nullptr)
	{
		// CreateWidget 함수로 새 위젯을 만들어 CurrentWidget에 저장
		CurrentWidget = CreateWidget<UPlayerDefaultWidget>(GetWorld(), NewWidgetClass);

		// 이후 뷰포트에 표시하도록 만들어준다.
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
		UE_LOG(Pro4, Log, TEXT("CurrentWidget : %s"), *CurrentWidget->GetName());
		UE_LOG(Pro4, Log, TEXT("Widget Changed"));
	}
}

void APro4PlayerController::DisplayLobby()
{
	ChangeMenuWidget(LobbyWidgetClass);
}

void APro4PlayerController::GameStart()
{
	ChangeMenuWidget(PlayerWidgetClass);
}
