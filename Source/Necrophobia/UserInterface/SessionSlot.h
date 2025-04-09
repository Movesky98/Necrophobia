// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "SessionSlot.generated.h"

DECLARE_DELEGATE_OneParam(FOnJoinSessionTriggered, const FOnlineSessionSearchResult& SearchResult);

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class NECROPHOBIA_API USessionSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USessionSlot(const FObjectInitializer& ObjectInitializer);

	FOnJoinSessionTriggered OnJoinSessionTriggered;

	void InitializeSessionData(const FOnlineSessionSearchResult& SearchResult);
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnJoinButtonClicked();
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	bool bIsInitalized;

	FName SessionName;

	int32 ConnectedPlayers;

	int32 MaxPlayers;

	int32 Ping;

	FOnlineSessionSearchResult SessionSearchResult;
};
