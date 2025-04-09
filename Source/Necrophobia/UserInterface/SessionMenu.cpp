// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenu.h"
#include "SessionSlot.h"
#include "PlayerSlot.h"

#include "GameFramework/CustomGameInstance.h"
#include "GameFramework/GISessionSubsystem.h"

#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"


USessionMenu::USessionMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<USessionSlot> WBP_SessionSlot(TEXT("/Game/Blueprints/WBP_SesisonSlot"));
	if (WBP_SessionSlot.Succeeded())
	{
		SessionSlotClass = WBP_SessionSlot.Class;
	}
}

void USessionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(NumberOfPlayersSlideBar != nullptr)) return;
	if (!ensure(SlideBarText != nullptr)) return;
	if (!ensure(SessionNameEditableText != nullptr)) return;
	if (!ensure(IsLANCheckBox != nullptr)) return;
	if (!ensure(CreateButton != nullptr)) return;
	if (!ensure(FindSessionsButton != nullptr)) return;
	if (!ensure(SessionScrollBox != nullptr)) return;

	UE_LOG(LogOnlineSubsystem, Warning, TEXT("Widget's Delegate Binding is successful."));

	LoadingThrobber->SetVisibility(ESlateVisibility::Hidden);
	NumberOfPlayersSlideBar->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderBarValueChanged);
	CreateButton->OnClicked.AddDynamic(this, &ThisClass::OnCreateButtonClicked);
	FindSessionsButton->OnClicked.AddDynamic(this, &ThisClass::OnFindButtonClicked);
}

void USessionMenu::SetUp()
{
	this->AddToViewport();

	FInputModeUIOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void USessionMenu::TearDown()
{
	this->RemoveFromParent();

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

void USessionMenu::OnSliderBarValueChanged(float Value)
{
	if (Value < 0) return;

	int32 PlayerNum = (int32)Value;
	FString TextStr = FString::FromInt(PlayerNum);

	SlideBarText->SetText(FText::FromString(TextStr));
}

void USessionMenu::OnCreateButtonClicked()
{
	FText Text = FText::FromString(FString(""));
	if (SessionNameEditableText->GetText().EqualTo(Text))
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("Please enter the session name."));
		return;
	}

	bool bIsLANMatch = IsLANCheckBox->GetCheckedState() == ECheckBoxState::Checked ? true : false;
	FName SessionName = FName(*SessionNameEditableText->GetText().ToString());
	int32 MaxPlayers = (int32)NumberOfPlayersSlideBar->GetValue();

	UGISessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (!SessionSubsystem)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("Check GameInstance's subsystem."));
		return;
	}

	SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ThisClass::OnCreateSessionCompleted);
	SessionSubsystem->CreateSession(MaxPlayers, SessionName, true);
}

void USessionMenu::OnCreateSessionCompleted(bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
		
	UE_LOG(LogOnlineSubsystem, Warning, TEXT("Create Session is successed."));

	UGISessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (!SessionSubsystem)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionMenu:OnCreateSessionCompleted > Check GameInstance's subsystem."));
		return;
	}

	SessionSubsystem->OnCreateSessionCompleteEvent.RemoveDynamic(this, &ThisClass::OnCreateSessionCompleted);
}


void USessionMenu::OnFindButtonClicked()
{
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		int32 MaxSearchResults = 1000;
		FindSessionDelegateHandle = GISSubsystem->OnFindSessionCompleteEvent.AddUObject(this, &ThisClass::OnFindSessionsComleted);
		GISSubsystem->FindSessions(MaxSearchResults, true);

		LoadingThrobber->SetVisibility(ESlateVisibility::Visible);
	}
}

void USessionMenu::OnFindSessionsComleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	LoadingThrobber->SetVisibility(ESlateVisibility::Hidden);

	if (SearchResults.Num() == 0)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionMenu: There's no sessions in UpdateSessions."));
		return;
	}

	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (GISSubsystem)
	{
		GISSubsystem->OnFindSessionCompleteEvent.Remove(FindSessionDelegateHandle);
	}

	for (const FOnlineSessionSearchResult& Result : SearchResults)
	{
		USessionSlot* SessionSlot = CreateWidget<USessionSlot>(GetWorld(), SessionSlotClass);

		SessionSlot->InitializeSessionData(Result);
		SessionScrollBox->AddChild(SessionSlot);
		SessionSlot->OnJoinSessionTriggered.BindUObject(this, &ThisClass::JoinSession);
	}
}

void USessionMenu::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (!GISSubsystem)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionMenu: JoinSession() > There's no GameInstanceSessionSubsystem."));
		return;
	}

	FString SessionName;
	SearchResult.Session.SessionSettings.Get(FName("NAME"), SessionName);

	JoinSessionDelegateHandle = GISSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &ThisClass::OnJoinSessionCompleted);
	GISSubsystem->JoinSession(SearchResult, FName(*SessionName));
}

void USessionMenu::OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result)
{
	UGISessionSubsystem* GISSubsystem = GetGameInstance()->GetSubsystem<UGISessionSubsystem>();
	if (!GISSubsystem)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionMenu: OnJoinSessionCompleted() > There's no GameInstanceSessionSubsystem."));
		return;
	}

	GISSubsystem->OnJoinSessionCompleteEvent.Remove(JoinSessionDelegateHandle);
}