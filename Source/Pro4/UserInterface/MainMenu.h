// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "WidgetMenu.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FSessionData {
	GENERATED_BODY()

	uint32 Index;
	FString SessionName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	bool isFull;
};

UCLASS()
class PRO4_API UMainMenu : public UWidgetMenu
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PreferenceButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameRecordButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NicknameField;

	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void LoadPreference();

	UFUNCTION()
	void LoadGameRecord();

	UFUNCTION()
	void ExitGame();


};
