// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSlot.generated.h"

class UTextBlock;
class UCheckBox;

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API UPlayerSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerSlot(const FObjectInitializer& ObjectInitializer);

	void InitializePlayerInfo(const FName& Name, bool bIsNewReady);

	void UpdateReadyCheckBox(bool bIsNewReady);

	FString GetPlayerName();
	
protected:
	virtual void NativeConstruct() override;
	
	FName PlayerName;

	bool bIsReady;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* IsReadyCheckBox;
};
