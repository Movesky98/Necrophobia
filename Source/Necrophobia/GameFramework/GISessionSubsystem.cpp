// Fill out your copyright notice in the Description page of Project Settings.


#include "GISessionSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"

#define NAME_GAMESESSION FName("Necrophobia")

DEFINE_LOG_CATEGORY(LogOnlineSubsystem);

UGISessionSubsystem::UGISessionSubsystem() 
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)),
	UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted)),
	RegisterPlayersCompleteDelegate(FOnRegisterPlayersCompleteDelegate::CreateUObject(this, &ThisClass::OnRegisterPlayersCompleted)),
	UnregisterPlayersCompleteDelegate(FOnUnregisterPlayersCompleteDelegate::CreateUObject(this, &ThisClass::OnUnregisterPlayersCompleted))
{

}

void UGISessionSubsystem::CreateSession(int32 NumPublicConnections, FName SessionName, bool IsLANMatch)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("UGISSubsystem: CreateSession() > There's no SessionInterface."));
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->Set(FName("NAME"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, *LastSessionSettings))
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("UGISSubsystem: CreateSession() > CreateSession is failed."));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UGISessionSubsystem::UpdateSession()
{
	const IOnlineSessionPtr	SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}
	TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	
	UpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);
	if (!SessionInterface->UpdateSession(LastSessionName, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void UGISessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		OnFindSessionCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		OnFindSessionCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UGISessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult, FName SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

FName UGISessionSubsystem::GetSessionName()
{
	if (LastSessionName.IsValid())
	{
		return LastSessionName;
	}

	return FName();
}

int32 UGISessionSubsystem::GetCurrentPlayer()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("UGISSubsystem: GetPlayerCount() > There is no session interface."));
		return -1;
	}

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(LastSessionName);
	if (Session)
	{
		return Session->RegisteredPlayers.Num();
	}

	return -1;
}

int32 UGISessionSubsystem::GetMaxPlayer()
{
	if (LastSessionSettings.IsValid())
	{
		return LastSessionSettings->NumPublicConnections;
	}

	return -1;
}


FOnlineSessionSettings UGISessionSubsystem::GetSessionSettings()
{
	if (LastSessionSettings.IsValid())
	{
		return *LastSessionSettings;
	}

	return FOnlineSessionSettings();
}

TArray<FUniqueNetIdRef> UGISessionSubsystem::GetRegisteredPlayers()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("UGISSubsystem: GetRegisteredPlayers() > There is no session interface."));
		return TArray<FUniqueNetIdRef>();
	}

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(LastSessionName);
	if (Session)
	{
		return Session->RegisteredPlayers;
	}

	return TArray<FUniqueNetIdRef>();
}

void UGISessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());

	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	UE_LOG(LogOnlineSubsystem, Warning, TEXT("GISSubsystem: OnCreateSessionCompleted() > Session creation was successful."));

	LastSessionName = SessionName;

	OnCreateSessionCompleteEvent.Broadcast(bWasSuccessful);

	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Levels/LV_LobbyMenu?listen");
	}
}

void UGISessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if(SessionInterface)
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(true);
}

void UGISessionSubsystem::OnFindSessionsCompleted(bool bWasSuccessful)
{
	const IOnlineSessionPtr	SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() == 0)
	{
		OnFindSessionCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), bWasSuccessful);
		return;
	}

	OnFindSessionCompleteEvent.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UGISessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	LastSessionName = SessionName;
	LastSessionSettings = MakeShareable(SessionInterface->GetSessionSettings(LastSessionName));

	// TODO : JoinSession 성공시, 플레이어 등록 실행.
	// 1. 플레이어 등록 실행 (GameState)
	// 2. 서버에서 플레이어 등록 성공 시, 플레이어 정보를 뿌려줌
	// 3. 뿌려지는 것이 성공했을 때, OnJoinSessionCompleteEvent -> Broadcast

	OnJoinSessionCompleteEvent.Broadcast(Result);

	TryTravelToCurrentSession(SessionName);
}

void UGISessionSubsystem::OnRegisterPlayersCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful)
{
	UE_LOG(LogOnlineSubsystem, Warning, TEXT("UGISSubsystem:: OnRegisterPlayersCompleted() >> is called."));

	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayersCompleteDelegateHandle);
	}

	if (!bWasSuccessful)
	{
		OnRegisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
		return;
	}

	if (SessionName == LastSessionName)
	{
		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(LastSessionName);

		if (Session)
			OnRegisterPlayersComplete.Broadcast(Session->RegisteredPlayers);
		else
			OnRegisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
	}
	else
	{
		FString Log = FString("GISSubsystem:: OnRegisterPlayersCompleted() >> SessionName and LastSessionName is not matched.");
		Log += FString("Current Session Name : ") + SessionName.ToString() + FString("\nLast Session Name : ") + LastSessionName.ToString();
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("%s"), *Log);

		OnRegisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
	}
}

void UGISessionSubsystem::OnUnregisterPlayersCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayersCompleteDelegateHandle);
	}

	if (!bWasSuccessful)
	{
		OnUnregisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
		return;
	}

	if (SessionName == LastSessionName)
	{
		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(LastSessionName);

		if (Session)
			OnUnregisterPlayersComplete.Broadcast(Session->RegisteredPlayers);
		else
			OnUnregisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
	}
	else
	{
		FString Log = FString("GISSubsystem:: OnRegisterPlayersCompleted() >> SessionName and LastSessionName is not matched.");
		Log += FString("Current Session Name : ") + SessionName.ToString() + FString("\nLast Session Name : ") + LastSessionName.ToString();
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("%s"), *Log);

		OnUnregisterPlayersComplete.Broadcast(TArray<FUniqueNetIdRef>());
	}
}

bool UGISessionSubsystem::TryTravelToCurrentSession(const FName SessionName)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		return false;
	}

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		return false;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
	return true;
}

void UGISessionSubsystem::RegisterPlayer(const FUniqueNetIdRepl& PlayerId, bool bWasInvited)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("GISSubsystem: RegisterPlayer() > There is no session interface."));
		return;
	}

	RegisterPlayersCompleteDelegateHandle = SessionInterface->AddOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayersCompleteDelegate);

	if (!SessionInterface->RegisterPlayer(LastSessionName, *PlayerId, bWasInvited))
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("GISSubsystem:: RegisterPlayer() >> Registering player is failed."));
	}
}

void UGISessionSubsystem::UnregisterPlayer(const FUniqueNetId& PlayerId)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("GISSubSystem:: UnregisterPlayer() >> There is no session interface."));
		return;
	}

	UnregisterPlayersCompleteDelegateHandle = SessionInterface->AddOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayersCompleteDelegate);

	if (!SessionInterface->UnregisterPlayer(LastSessionName, PlayerId))
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("GISSubsystem:: UnregisterPlayer() >> Registering player is failed."));
	}
}
