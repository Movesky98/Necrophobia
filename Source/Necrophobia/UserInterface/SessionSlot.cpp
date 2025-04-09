// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlot.h"
#include "SessionMenu.h"
#include "GameFramework/GISessionSubsystem.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

USessionSlot::USessionSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bIsInitalized(false), SessionName(FName("")), ConnectedPlayers(0), MaxPlayers(0), Ping(0)
{

}

void USessionSlot::InitializeSessionData(const FOnlineSessionSearchResult& SearchResult)
{
	if (!SearchResult.IsValid())
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionSlot: InitializeSessionData() > The SearchResult parameter has and invalid value."));
		return;
	}
	FString Name;

	SessionSearchResult = SearchResult;
	SessionSearchResult.Session.SessionSettings.Get(FName("NAME"), Name);
	SessionName = FName(*Name);
	MaxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
	if (MaxPlayers <= 0 || MaxPlayers < ConnectedPlayers)
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("USessionSlot: InitializeSessionData() > The max players count was assigned an incorrect value during session initialization."));
		return;
	}

	ConnectedPlayers = MaxPlayers - SessionSearchResult.Session.NumOpenPublicConnections;
	if (ConnectedPlayers <= 0)
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("USessionSlot: InitializeSessionData() > The connected players count was assigned an incorrect value during session initialization."));
		return;
	}

	Ping = SessionSearchResult.PingInMs;
	if (Ping <= 0)
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("USessionSlot: InitializeSessionData() > The session's ping was assigned an incorrect value during session initialization."));
		return;
	}

	FString LogStr = FString("USessionSlot: InitializeSessionData() > \n");
	LogStr += FString("SessionName : ") + Name + FString("\n");
	LogStr += FString("MaxPlayers : ") + FString::FromInt(MaxPlayers) + FString("\n");
	LogStr += FString("ConnectedPlayers : ") + FString::FromInt(ConnectedPlayers) + FString("\n");
	LogStr += FString("Ping : ") + FString::FromInt(Ping);

	UE_LOG(LogOnlineSubsystem, Warning, TEXT("%s"), *LogStr);

	bIsInitalized = true;
}

void USessionSlot::NativeConstruct()
{
	Super::NativeConstruct();

	check(SessionNameText != nullptr);
	check(PingText != nullptr);
	check(PlayerCountText != nullptr);
	check(JoinButton != nullptr);

	if (!bIsInitalized)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("USessionSlot: NativeConstruct() > Please initialize session information."));
		return;
	}

	SessionNameText->SetText(FText::FromName(SessionName));
	FString PlayerCountStr = FString::FromInt(ConnectedPlayers) + " / " + FString::FromInt(MaxPlayers);
	PlayerCountText->SetText(FText::FromString(PlayerCountStr));
	PingText->SetText(FText::FromString(FString::FromInt(Ping)));

	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
}

void USessionSlot::OnJoinButtonClicked()
{
	// TODO : Join Session.

	OnJoinSessionTriggered.Execute(SessionSearchResult);
}
