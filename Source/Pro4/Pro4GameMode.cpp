// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pro4GameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include <Blueprint/UserWidget.h>

APro4GameMode::APro4GameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APro4Character::StaticClass();
	PlayerControllerClass = APro4PlayerController::StaticClass();
}

void APro4GameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작될 때 UI를 표시하도록 만들어줌.
	ChangeMenuWidget(StartingWidgetClass);
}

void APro4GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
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
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);

		// 이후 뷰포트에 표시하도록 만들어준다.
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
