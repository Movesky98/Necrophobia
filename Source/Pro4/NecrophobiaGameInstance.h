// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "Engine/GameInstance.h"
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
	void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadPlayerMenu();

private:
	TSubclassOf<class UUserWidget> MainClass;
	TSubclassOf<class UUserWidget> PlayerClass;

	class UMainMenu* Menu;
	
};
