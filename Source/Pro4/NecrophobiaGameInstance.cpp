// Fill out your copyright notice in the Description page of Project Settings.

#include "NecrophobiaGameInstance.h"
#include "Engine/Engine.h"

#include "UObject/ConstructorHelpers.h"

#include "UserInterface/MainMenu.h"
#include "UserInterface/PlayerMenu.h"

UNecrophobiaGameInstance::UNecrophobiaGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(Pro4, Warning, TEXT("GameInstance is Spawned"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WBPMainClass(TEXT("/Game/UI/MainMenu/WBP_MainMenu"));
	if (!ensure(WBPMainClass.Class != nullptr)) return;
	
	this->MainClass = WBPMainClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> WBPPlayerClass(TEXT("/Game/UI/Player/WBP_PlayerMenu"));
	if (!ensure(WBPPlayerClass.Class != nullptr)) return;

	this->PlayerClass = WBPPlayerClass.Class;
}

void UNecrophobiaGameInstance::Init()
{
	UE_LOG(Pro4, Warning, TEXT("MainMenu : %s"), *MainClass->GetName());
	UE_LOG(Pro4, Warning, TEXT("MainMenu : %s"), *PlayerClass->GetName());
}

void UNecrophobiaGameInstance::Host()
{
	if (Menu != nullptr)
	{
		// 메뉴가 (현재 UI가) 켜져있다면 분해시킴
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Host Successed."));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UNecrophobiaGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		// 메뉴가 (현재 UI가) 켜져있다면 분해시킴
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UNecrophobiaGameInstance::LoadMenu()
{
	UE_LOG(Pro4, Warning, TEXT("Menu Loading..."));
	// 메인메뉴가 비어있을 경우
	if (!ensure(MainClass != nullptr)) return;
	
	Menu = CreateWidget<UMainMenu>(this, MainClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->SetUp();
	Menu->SetMenuInterface(this);
}

void UNecrophobiaGameInstance::LoadPlayerMenu()
{
	if (!ensure(PlayerClass != nullptr)) return;

	UPlayerMenu* PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerClass);
	if (!ensure(PlayerMenu != nullptr)) return;

	PlayerMenu->SetUp();
	PlayerMenu->SetMenuInterface(this);
}