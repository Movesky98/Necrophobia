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

	// OnlineSubSystem�� ������
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	// OnlineSubSystem�� �ִٸ�
	if (Subsystem != nullptr)
	{
		UE_LOG(Pro4, Warning, TEXT("Found Subsystem %s."), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		// SessionInterface�� �����Ѵٸ�
		if (SessionInterface != nullptr)
		{
			UE_LOG(Pro4, Warning, TEXT("Found Session Interface."));

			/*
			* ���� ����, �ı�, ���� ���� �Ϸ�� ��� �˷��ִ� ��������Ʈ ���
			* �� ���, �Ķ���ͷ� �ѱ� �Լ����� ����Ǿ� ���� ���� �Լ��� �Ϸ���� �� �� ����.
			* �ݹ鿡 ����� ������!
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

// MainMenu���� '���� ����' ��ư�� ������ ����
void UNecrophobiaGameInstance::Host()
{
	UE_LOG(Pro4, Warning, TEXT("Host the Server."));

	// ���� �������̽��� ���� ���
	if (SessionInterface.IsValid())
	{
		// ������ �����ϱ� ��, �ش� �̸��� ������ �ִ� ������ �̹� �����ϴ��� Ȯ��.
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		// �̹� 'Necrophobia' �̸��� ���� ������ �����Ѵٸ� ����
		// ���ٸ� ������ ������.
		if (ExistingSession != nullptr)
		{
			// SESSION_NAME('Necrophobia')�� �̸��� ���� ���� ����
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			// ���� ����
			CreateSession();
		}
	}
}

/* Session ���� �Լ� */
void UNecrophobiaGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		// ���� ���� ����
		FOnlineSessionSettings SessionSettings;

		// �¶��� ����ý����� 'NULL'�� ����� ���, �������θ� ��Ƽ�÷��̸� �׽�Ʈ�� �� ����.
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;

		SessionSettings.NumPublicConnections = 5;	// ���ǿ��� ����� �� �ִ� �ο� ��.
		SessionSettings.bShouldAdvertise = true;	// �¶��ο��� �ش� ������ ���̵��� ��.

		// �̹� �������� ���ǿ� ���� �������� ����
		SessionSettings.bAllowJoinInProgress = true;
		// Presence�� ����ϸ鼭, �÷��̾���� Presence�� ���� ������ ����ϵ��� ��.
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinViaPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

/* ���� ����(���� ����)�� ����Ǵ� �Լ� */
void UNecrophobiaGameInstance::Join(uint32 Index)
{
	UE_LOG(Pro4, Warning, TEXT("Join the Server."));

	if (Menu != nullptr)
	{
		// �޴�(MainMenu)�� �����ִٸ� ���ؽ�Ŵ
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

/* ������ ���ǵ��� ã�� �Լ� */
void UNecrophobiaGameInstance::FindSessionList()
{
	// ������ ã�� ���� �˻� ���� ����
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

/* ������ �� �ִ� ������ �ִٸ� true, �ƴ϶�� false�� ��ȯ */
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
	
	// ���� ������ ������ ���õ��� ���� ���
	return false;
}

/* ������ ���۵ǰ� ���ο� ������ ������ �������� ����Ǵ� �Լ� */
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
	// ���θ޴��� ������� ���
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

// [�� ����] -> [���� �޴�]�� �̵��� �� ����ϴ� �Լ�. ����� ���̰� ���� ����. (�� ����)
void UNecrophobiaGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

#pragma endregion

#pragma region SessionDelegates
/* ���� ������ �Ϸ�ǰ� ����Ǵ� �Լ� */
void UNecrophobiaGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(Pro4, Warning, TEXT("Could not CreateSession"));
		return;
	}

	if (Menu != nullptr)
	{
		// �޴��� (���� UI��) �����ִٸ� ���ؽ�Ŵ
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
	// �ı��� �Ϸ�Ǿ��ٸ�, CreateSession
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

		// TArray ������ SearchResults �ݺ�
		// auto�� �̿��� Ÿ���� �߷��ϰ� &�� �ٿ� ������ ����ϵ��� ��.
		// auto�� �̿��ؼ� Ÿ���� �˰ԵǸ� �� Ÿ������ �̸� �������ָ� ��!��!
		if (SessionSearch->SearchResults.Num() == 0)
		{
			UE_LOG(Pro4, Warning, TEXT("There are no Sessions."))
		}
		else
		{
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				// ������ ID�� ������
				UE_LOG(LogTemp, Warning, TEXT("Found Session Name : %s"), *SearchResult.GetSessionIdStr());
				FSessionData Data;
				Data.SessionName = SearchResult.GetSessionIdStr();
				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;	// [�ִ� �÷��̾� ��] - [���� ���� ������ �÷��̾� ���� ��]
				Data.isFull = (Data.CurrentPlayers == Data.MaxPlayers) ? true : false;
				Data.Index = i++;

				// ã�� ���ǵ��� ������ ServerInfo�� ����
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

/* ������ ���ǿ��� ȣ��Ʈ�� �����Ͽ��� ��, Ŭ���̾�Ʈ���� ���� ���� ������ ���� �Լ� */
void UNecrophobiaGameInstance::OnNetworkFailureComplete(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}
#pragma endregion
