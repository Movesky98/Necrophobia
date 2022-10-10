// Fill out your copyright notice in the Description page of Project Settings.

#include "NecrophobiaGameInstance.h"
#include "Pro4PlayerController.h"
#include "UserInterface/PlayerMenu.h"

#include "Engine/Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"


const static FName SESSION_NAME = TEXT("Necrophobia");

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
	UE_LOG(Pro4, Warning, TEXT("MainMenu : %s."), *MainClass->GetName());
	UE_LOG(Pro4, Warning, TEXT("PlayerMenu : %s."), *PlayerClass->GetName());

	// OnlineSubSystem을 가져옴
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	// OnlineSubSystem이 있다면
	if (Subsystem != nullptr)
	{
		UE_LOG(Pro4, Warning, TEXT("Found Subsystem %s."), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		// SessionInterface가 존재한다면
		if (SessionInterface != nullptr)
		{
			UE_LOG(Pro4, Warning, TEXT("Found Session Interface."));

			/*
			* 세션 생성, 파괴, 참여 등이 완료될 경우 알려주는 델리게이트 등록
			* 이 경우, 파라미터로 넘긴 함수들이 실행되어 세션 관련 함수가 완료됨을 알 수 있음.
			* 콜백에 등록한 상태임!
			*/
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNecrophobiaGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNecrophobiaGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNecrophobiaGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNecrophobiaGameInstance::OnJoinSessionComplete);
		}
	}
	else 
	{
		UE_LOG(Pro4, Warning, TEXT("Subsystem not found."));
	}

	if (GEngine != nullptr) 
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UNecrophobiaGameInstance::OnNetworkFailureComplete);
	}
	
}

#pragma region OnlineSubsystem

// MainMenu에서 '게임 시작' 버튼을 누르면 실행
void UNecrophobiaGameInstance::Host()
{
	UE_LOG(Pro4, Warning, TEXT("Host the Server."));

	// 세션 인터페이스가 있을 경우
	if (SessionInterface.IsValid())
	{
		// 세션을 생성하기 전, 해당 이름을 가지고 있는 세션이 이미 존재하는지 확인.
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		// 이미 'Necrophobia' 이름을 가진 세션이 존재한다면 제거
		// 없다면 세션을 생성함.
		if (ExistingSession != nullptr)
		{
			// SESSION_NAME('Necrophobia')의 이름을 가진 세션 제거
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			// 세션 생성
			CreateSession();
		}
	}
}

/* Session 생성 함수 */
void UNecrophobiaGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		// 세션 기초 설정
		FOnlineSessionSettings SessionSettings;

		// 온라인 서브시스템이 'NULL'로 실행될 경우, 랜서버로만 멀티플레이를 테스트할 수 있음.
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;

		SessionSettings.NumPublicConnections = 5;	// 세션에서 연결될 수 있는 인원 수.
		SessionSettings.bShouldAdvertise = true;	// 온라인에서 해당 세션이 보이도록 함.

		// 이미 실행중인 세션에 참가 가능한지 여부
		SessionSettings.bAllowJoinInProgress = true;
		// Presence를 사용하면서, 플레이어들이 Presence를 통한 참가를 허용하도록 함.
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinViaPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

/* 세션 참가(서버 참가)시 실행되는 함수 */
void UNecrophobiaGameInstance::Join(uint32 Index)
{
	UE_LOG(Pro4, Warning, TEXT("Join the Server."));

	if (Menu != nullptr)
	{
		// 메뉴(MainMenu)가 켜져있다면 분해시킴
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

/* 생선된 세션들을 찾는 함수 */
void UNecrophobiaGameInstance::FindSessionList()
{
	// 세션을 찾기 위한 검색 세팅 설정
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(Pro4, Warning, TEXT("Starting Find Session."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

/* 입장할 수 있는 세션이 있다면 true, 아니라면 false를 반환 */
bool UNecrophobiaGameInstance::CheckSession(TArray<FSessionData> ServerInfo)
{
	for (const FSessionData& ServerData : ServerInfo)
	{
		if (!ServerData.isFull)
		{
			SessionIndex = ServerData.Index;
			return true;
		}
	}
	
	// 입장 가능한 세션이 선택되지 않은 경우
	return false;
}

/* 게임이 시작되고 새로운 유저의 참가를 막기위해 실행되는 함수 */
void UNecrophobiaGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

#pragma endregion

#pragma region LoadMenu

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

	PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerClass);
	if (!ensure(PlayerMenu != nullptr)) return;

	PlayerMenu->SetUp();
	PlayerMenu->SetMenuInterface(this);
}

// [인 게임] -> [메인 메뉴]로 이동할 때 사용하는 함수. 현재는 쓰이고 있지 않음. (쓸 예정)
void UNecrophobiaGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

#pragma endregion

#pragma region SessionDelegates
/* 세션 생성이 완료되고 실행되는 함수 */
void UNecrophobiaGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(Pro4, Warning, TEXT("Could not CreateSession"));
		return;
	}

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

void UNecrophobiaGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	// 파괴가 완료되었다면, CreateSession
	if (Success)
	{
		CreateSession();
	}
}

void UNecrophobiaGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FSessionData> ServerInfo;
		uint32 i = 0;

		// TArray 형식인 SearchResults 반복
		// auto를 이용해 타입을 추론하고 &을 붙여 참조를 사용하도록 함.
		// auto를 이용해서 타입을 알게되면 그 타입으로 이름 변경해주면 깰!꼼!
		if (SessionSearch->SearchResults.Num() == 0)
		{
			UE_LOG(Pro4, Warning, TEXT("There are no Sessions."))
		}
		else
		{
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				// 세션의 ID를 가져옴
				UE_LOG(LogTemp, Warning, TEXT("Found Session Name : %s"), *SearchResult.GetSessionIdStr());
				FSessionData Data;
				Data.SessionName = SearchResult.GetSessionIdStr();
				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;	// [최대 플레이어 수] - [현재 입장 가능한 플레이어 슬롯 수]
				Data.isFull = (Data.CurrentPlayers == Data.MaxPlayers) ? true : false;
				Data.Index = i++;

				// 찾은 세션들의 정보를 ServerInfo에 저장
				ServerInfo.Add(Data);
			}
		}
		

		if (CheckSession(ServerInfo))
		{
			Join(SessionIndex);
		}
		else
		{
			Host();
		}
	}
}

void UNecrophobiaGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

/* 생성된 세션에서 호스트가 퇴장하였을 때, 클라이언트들의 멈춤 현상 방지를 위한 함수 */
void UNecrophobiaGameInstance::OnNetworkFailureComplete(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}
#pragma endregion
