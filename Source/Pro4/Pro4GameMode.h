// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Pro4.h"
#include "GameFramework/GameModeBase.h"
#include "Pro4GameMode.generated.h"

UCLASS()
class APro4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APro4GameMode();

	UFUNCTION(BlueprintCallable, Category=UMG_Game)
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	void BeginPlay() override;

	// 게임이 시작될 때 표시될 UI 위젯을 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> StartingWidgetClass;
	
	// 현재 화면에 표시되고 있는 UI 위젯을 저장하고 있을 포인터 변수
	UPROPERTY()
	UUserWidget* CurrentWidget;
};



