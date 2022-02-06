// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4PlayerController.h"

void APro4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SetInputMode에 FInputModeGameAndUI를 넣어주면
	// 게임과 UI 모두 입력 가능한 상태가 된다.
	SetInputMode(FInputModeGameOnly());
}