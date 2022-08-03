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
	* ���� ��ġ����ŷ�� �����ϱ⿣ �Ƿ��� �����ϹǷ� �켱 �������� �������� �����Ͽ�
	* ��Ƽ�÷��� �׽�Ʈ�� ���ֽñ� �ٶ��ϴ�.
	* ���߿� ��ġ����ŷ�� �Ǵ´�� �Լ��� �����ؼ� �ø��ڽ��ϴ�.
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
