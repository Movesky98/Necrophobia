// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"

#include "WidgetMenu.h"

#include "PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UPlayerMenu : public UWidgetMenu
{
	GENERATED_BODY()
	
public:
	void SetUp();

	virtual bool Initialize();

	void SetTimeText(uint16 min, uint16 sec);

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* InGameTimeText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Time_ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Armor_ProgressBar;
};
