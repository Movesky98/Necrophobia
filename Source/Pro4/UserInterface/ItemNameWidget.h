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
public:
	UItemNameWidget(const FObjectInitializer& ObjectInitializer);

	void ToggleVisibility();
	virtual bool Initialize();

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString& _ItemName);

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ItemName;


};
