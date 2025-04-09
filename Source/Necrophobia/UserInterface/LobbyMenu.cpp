// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "GameFramework/SessionGameState.h"
#include "GameFramework/SessionPlayerController.h"
#include "GameFramework/SessionPlayerState.h"
#include "UserInterface/PlayerSlot.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

ULobbyMenu::ULobbyMenu(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer), 
	OnReadyStateUpdatedDelegate(FOnReadyStateUpdatedDelegate::CreateUObject(this, &ThisClass::OnReadyStateUpdated))
{
	static ConstructorHelpers::FClassFinder<UPlayerSlot> WBP_PlayerSlot(TEXT("/Game/Blueprints/WBP_PlayerSlot"));
	if (WBP_PlayerSlot.Succeeded())
	{
		PlayerSlotClass = WBP_PlayerSlot.Class;
	}
}

void ULobbyMenu::UpdateParticipants()
{
	if (!SessionDataProvider) 
	{
		UE_LOG(LogTemp, Warning, TEXT("LobbyMenu:: UpdateParticipants() >> There's no SessionDataProvider."));
		return;
	}

	FName SessionName = SessionDataProvider->GetSessionName();
	int32 MaxPlayer = SessionDataProvider->GetMaxPlayer();
	int32 CurrentPlayer = SessionDataProvider->GetCurrentPlayer();
	FString PlayerCount = FString::FromInt(CurrentPlayer) + FString(" / ") + FString::FromInt(MaxPlayer);
	
	const TArray<FUniqueNetIdRepl>& PlayerList = SessionDataProvider->GetPlayerList();
	PlayerListScrollBox->ClearChildren();

	SessionNameText->SetText(FText::FromName(SessionName));
	PlayersText->SetText(FText::FromString(PlayerCount));
	
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	TArray<APlayerState*> PlayerStateArray = World->GetGameState()->PlayerArray;


	for(const FUniqueNetIdRepl& Player : PlayerList)
	{
		UPlayerSlot* PlayerSlot = CreateWidget<UPlayerSlot>(GetWorld(), PlayerSlotClass);
	
		FName PlayerName(Player.GetUniqueNetId()->ToString());

		bool bIsReady = false;
		for (APlayerState* PlayerState : PlayerStateArray)
		{
			ASessionPlayerState* SessionPS = Cast<ASessionPlayerState>(PlayerState);
			if (SessionPS && SessionPS->GetUniqueId() == Player)
			{
				bIsReady = SessionPS->GetIsReady();
				break;
			}
		}

		PlayerSlot->InitializePlayerInfo(PlayerName, bIsReady);

		PlayerListScrollBox->AddChild(PlayerSlot);
	}
}

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ASessionGameState* GameState = GetWorld()->GetGameState<ASessionGameState>();
	if (GameState)
	{
		SessionDataProvider = NewObject<USessionDataProvider>(this);
		SessionDataProvider->Initialize(GameState);
		SessionDataProvider->OnSessionDataUpdated.AddDynamic(this, &ThisClass::UpdateParticipants);
		OnReadyStateUpdatedDelegateHandle = SessionDataProvider->AddOnReadyStateUpdatedDelegate_Handle(OnReadyStateUpdatedDelegate);

		// UpdateSesisonInfo.
		UpdateParticipants();
	}

	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitSession);
	ReadyButton->OnClicked.AddDynamic(this, &ThisClass::Ready);
}

void ULobbyMenu::NativeDestruct()
{
	Super::NativeDestruct();

	TearDown();
}

void ULobbyMenu::SetUp()
{
	this->AddToViewport();

	FInputModeUIOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void ULobbyMenu::TearDown()
{
	this->RemoveFromParent();

	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

void ULobbyMenu::ExitSession()
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/Levels/LV_MainMenu", ETravelType::TRAVEL_Absolute);
	}
}

void ULobbyMenu::Ready()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ASessionPlayerController* SessionPC = Cast<ASessionPlayerController>(World->GetFirstPlayerController());
	if (SessionPC)
	{
		SessionPC->UpdateReadyState(true);
	}
}

void ULobbyMenu::OnReadyStateUpdated(const FUniqueNetIdRepl& UniqueNetId, bool bIsNewReady)
{
	for (UWidget* Widget : PlayerListScrollBox->GetAllChildren())
	{
		UPlayerSlot* PlayerSlot = Cast<UPlayerSlot>(Widget);
		if (PlayerSlot && PlayerSlot->GetPlayerName() == UniqueNetId.GetUniqueNetId()->ToString())
		{
			PlayerSlot->UpdateReadyCheckBox(bIsNewReady);
		}
	}
}
