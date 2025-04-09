// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interface/WidgetInterface.h"
#include "SessionMenu.generated.h"

class UWidgetSwitcher;
class UCanvasPanel;
class USlider;
class UTextBlock;
class UButton;
class UEditableText;
class UCheckBox;
class UScrollBox;
class UCircularThrobber;

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API USessionMenu : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()
	
public:
	USessionMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetUp() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void TearDown() override;

protected:
	UFUNCTION()
	void OnSliderBarValueChanged(float Value);

	UFUNCTION()
	void OnCreateButtonClicked();

	UFUNCTION()
	void OnCreateSessionCompleted(bool bWasSuccessful);
	
	UFUNCTION()
	void OnFindButtonClicked();

	void OnFindSessionsComleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);

	FDelegateHandle FindSessionDelegateHandle;

	void JoinSession(const FOnlineSessionSearchResult& SearchResult);

	void OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle JoinSessionDelegateHandle;

private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* DefaultPanel;

	UPROPERTY(meta = (BindWidget))
	USlider* NumberOfPlayersSlideBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlideBarText;

	UPROPERTY(meta = (BindWidget))
	UEditableText* SessionNameEditableText;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* IsLANCheckBox;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionsButton;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SessionScrollBox;

	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* LoadingThrobber;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Session", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USessionSlot> SessionSlotClass;
};
