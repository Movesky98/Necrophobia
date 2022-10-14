// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "Blueprint/UserWidget.h"
#include "ItemNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UItemNameWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ItemName;
};
