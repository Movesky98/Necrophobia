// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "NecrophobiaGameInstance.h"
#include "InGameState.h"
#include "UserInterface/PlayerMenu.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

/*
#include <Steamworks/Steamv153/sdk/public/steam/steam_api.h>
 ���� ���̺귯���� ��������� ������
#pragma comment(lib, "ThirdParty/Steamworks/Steamv153/sdk/redistributable_bin/win64/steam_api64.lib")
*/

AInGameMode::AInGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    // set default pawn class to our Blueprinted character
    DefaultPawnClass = APro4Character::StaticClass();
    PlayerControllerClass = APro4PlayerController::StaticClass();
    GameStateClass = AInGameState::StaticClass();
}

void AInGameMode::BeginPlay()
{
    Super::BeginPlay();

    InGameState = GetWorld()->GetGameState<AInGameState>();
    InGameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());

    if (InGameState != nullptr) 
    {
        UE_LOG(Pro4, Warning, TEXT("GameState : %s"), *InGameState->GetName());
    }
    else
    {
        UE_LOG(Pro4, Warning, TEXT("GameState is NULL"));
    }
}

void AInGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Time += DeltaTime;
    if (Time >= 1.0f)
    {
        Time--;
        InGameState->AddInGameSeconds();
        InGameInstance->PlayerMenu->SetTimeText(InGameState->GetInGameMinutes(), InGameState->GetInGameSeconds());
    }
}

/* �÷��̾ ���ǿ� �����ϰ� �α��� �� �� ����Ǵ� �Լ� */
void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ++NumberOfPlayers;
    UE_LOG(Pro4, Warning, TEXT("Reached Player"));

    if (NumberOfPlayers >= 3 && !isSetStartTimer)
    {
        UE_LOG(Pro4, Warning, TEXT("Start Timer..."));
        GetWorldTimerManager().SetTimer(GameStartTimer, this, &AInGameMode::CountingTheSeconds, 1.0f, true, 5.0f);
        isSetStartTimer = true;
    }

}

/* �÷��̾ ���ǿ��� ������ �� ����Ǵ� �Լ� */
void AInGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;

    if (isSetStartTimer && NumberOfPlayers == 1)
    {
        UE_LOG(Pro4, Warning, TEXT("Game Over."));
    }

}

/* ������ �����ϴ� �Լ� */
void AInGameMode::StartGame()
{
    UNecrophobiaGameInstance* GameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->StartSession();

    UEngine* Engine = GameInstance->GetEngine();
    if (Engine == nullptr) return;
    Engine->AddOnScreenDebugMessage(0, 10.0f, FColor::Red, TEXT("Game Start!"));
}

/* ����� �ο��� ���̰� �ð��ʸ� ���� �Լ� */
void AInGameMode::CountingTheSeconds()
{
    UE_LOG(Pro4, Warning, TEXT("Timer : %d"), CountSeconds);
    if (--CountSeconds <= 0)
    {
        StartGame();
        GetWorldTimerManager().ClearTimer(GameStartTimer);
    }
}