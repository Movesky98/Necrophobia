// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/WidgetInterface.h"
#include "Session/SessionDataProvider.h"
#include "LobbyMenu.generated.h"

class UButton;
class UTextBlock;
class UScrollBox;

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API ULobbyMenu : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()
	
public:
	ULobbyMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateParticipants();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetUp() override;

	UFUNCTION(BlueprintCallable)
	virtual void TearDown() override;

	UFUNCTION()
	void ExitSession();

	UFUNCTION()
	void Ready();

	UFUNCTION()
	void OnReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsNewReady);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayersText;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerListScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Session", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerSlot> PlayerSlotClass;

	USessionDataProvider* SessionDataProvider;

	FOnReadyStateUpdatedDelegate OnReadyStateUpdatedDelegate;
	FDelegateHandle OnReadyStateUpdatedDelegateHandle;
};
