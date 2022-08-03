// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "WidgetMenu.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UWidgetMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp();
	void Teardown();

	void SetMenuInterface(IMenuInterface* OtherMenuInterface);

protected:
	IMenuInterface* MenuInterface;
};
