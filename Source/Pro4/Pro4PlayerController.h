// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/PlayerController.h"
#include "Pro4PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API APro4PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APro4PlayerController();
	
	UFUNCTION(BlueprintCallable, Category = UMG_Game)
	void ChangeMenuWidget(TSubclassOf<class UPlayerDefaultWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = UMG_Game)
	void DisplayLobby();

	UFUNCTION(BlueprintCallable, Category = UMG_Game)
	void GameStart();
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPlayerDefaultWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPlayerDefaultWidget> LobbyWidgetClass;

	UPROPERTY()
	class UPlayerDefaultWidget* CurrentWidget;
};
