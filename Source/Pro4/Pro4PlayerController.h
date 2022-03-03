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
	
protected:
	void BeginPlay() override;

	// 게임이 시작될 때 표시될 UI 위젯을 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPlayerDefaultWidget> StartingWidgetClass;

	// 현재 화면에 표시되고 있는 UI 위젯을 저장하고 있을 포인터 변수
	UPROPERTY()
	class UPlayerDefaultWidget* CurrentWidget;
};
