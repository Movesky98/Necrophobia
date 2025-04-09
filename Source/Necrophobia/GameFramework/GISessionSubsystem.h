// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GISessionSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOnlineSubsystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGISOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGISOnUpdateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FGISOnFindSessionComplete, const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FGISOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_MULTICAST_DELEGATE_OneParam(FGISOnRegisterPlayersComplete, const TArray<FUniqueNetIdRef>& PlayerList);
DECLARE_MULTICAST_DELEGATE_OneParam(FGISOnUnregisterPlayersComplete, const TArray<FUniqueNetIdRef>& PlayerList);

/**
 * 
 */
UCLASS()
class NECROPHOBIA_API UGISessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGISessionSubsystem();

	void CreateSession(int32 NumPublicConnections, FName SessionName, bool IsLANMatch = true);

	FGISOnCreateSessionComplete OnCreateSessionCompleteEvent;

	void UpdateSession();

	FGISOnUpdateSessionComplete OnUpdateSessionCompleteEvent;

	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);

	FGISOnFindSessionComplete OnFindSessionCompleteEvent;

	void JoinSession(const FOnlineSessionSearchResult& SessionResult, FName SessionName);

	FGISOnJoinSessionComplete OnJoinSessionCompleteEvent;

	FName GetSessionName();

	int32 GetCurrentPlayer();

	int32 GetMaxPlayer();

	FOnlineSessionSettings GetSessionSettings();

	TArray<FUniqueNetIdRef> GetRegisteredPlayers();

	void RegisterPlayer(const FUniqueNetIdRepl& PlayerId, bool bWasInvited);

	void UnregisterPlayer(const FUniqueNetId& PlayerId);

	/* 플레이어 등록 완료 델리게이트 */
	FGISOnRegisterPlayersComplete OnRegisterPlayersComplete;

	FGISOnUnregisterPlayersComplete OnUnregisterPlayersComplete;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);

	void OnUpdateSessionCompleted(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsCompleted(bool bWasSuccessful);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnRegisterPlayersCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful);

	void OnUnregisterPlayersCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful);

	bool TryTravelToCurrentSession(const FName SessionName);

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	FName LastSessionName;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	FOnRegisterPlayersCompleteDelegate RegisterPlayersCompleteDelegate;
	FDelegateHandle RegisterPlayersCompleteDelegateHandle;

	FOnUnregisterPlayersCompleteDelegate UnregisterPlayersCompleteDelegate;
	FDelegateHandle UnregisterPlayersCompleteDelegateHandle;

	/* OnlineSubsystem NULL에서는 사용되지 않음. (아마도) */
	//FOnSessionParticipantJoinedDelegate SessionParticipantJoinedDelegate;
	//FDelegateHandle SessionParticipantJoinedDelegateHandle;
};
