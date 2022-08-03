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
	* 지금 매치메이킹을 구현하기엔 실력이 부족하므로 우선 리슨서버 형식으로 구현하여
	* 멀티플레이 테스트를 해주시길 바랍니다.
	* 나중에 매치메이킹이 되는대로 함수를 수정해서 올리겠습니다.
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
