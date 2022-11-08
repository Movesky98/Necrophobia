// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"

#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "UserInterface/MainMenu.h"
#include "UserInterface/MenuInterface.h"
#include "NecrophobiaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API UNecrophobiaGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UNecrophobiaGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	/* 
	* I'm not good enough to implement matchmaking now, so I'll implement it as a listen server first
	* Please do a multi-play test.
	* I will revise the function and upload it as soon as the matchmaking is done.
	*/
	UFUNCTION()
	void Host() override;

	UFUNCTION()
	void Join(uint32 Index) override;

	UFUNCTION()
	void FindSessionList() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadPlayerMenu();

	void LoadMainMenu() override;

	void StartSession();

	void NoticeToPlayerControllerTimeChanged();

	class UPlayerMenu* PlayerMenu;

	class UAudioComponent* BGM;
	class USoundCue* SBGM;

private:
	TSubclassOf<class UUserWidget> MainClass;
	TSubclassOf<class UUserWidget> PlayerClass;

	class UMainMenu* Menu;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	uint32 SessionIndex = -1;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailureComplete(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();
	bool CheckSession(TArray<FSessionData> ServerInfo);
};
