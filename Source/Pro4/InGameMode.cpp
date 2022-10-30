// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "Pro4Boss.h"
#include "NecrophobiaGameInstance.h"
#include "InGameState.h"
#include "InGamePlayerState.h"
#include "UserInterface/PlayerMenu.h"


#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

/*
#include <Steamworks/Steamv153/sdk/public/steam/steam_api.h>
 스팀 라이브러리를 명시적으로 연결함
#pragma comment(lib, "ThirdParty/Steamworks/Steamv153/sdk/redistributable_bin/win64/steam_api64.lib")
*/

AInGameMode::AInGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    // set default pawn class to our Blueprinted character

    // static ConstructorHelpers::FClassFinder<APro4Character> BP_PlayerCharacter(TEXT("/Game/BLUEPRINT(JunJae)/BP_Pro4Character"));

    /*if (BP_PlayerCharacter.Succeeded())   BP_PlayerCharacter은 아예 C++로 구현해놓겠음.
    {
        DefaultPawnClass = BP_PlayerCharacter.Class;
        UE_LOG(Pro4, Warning, TEXT("Set PawnClass : BP_PlayerCharacter"));
    }
    else
    {
    
    }*/

    DefaultPawnClass = APro4Character::StaticClass();
    UE_LOG(Pro4, Warning, TEXT("Set PawnClass : APro4Character"));
    PlayerControllerClass = APro4PlayerController::StaticClass();
    GameStateClass = AInGameState::StaticClass();
    PlayerStateClass = AInGamePlayerState::StaticClass();
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

        if (InGameState->GetIsTimeToSpawnBoss())
        {
            SpawnBossZombie();
        }
    }
}

/* 플레이어가 세션에 입장하고 로그인 한 후 실행되는 함수 */
void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ++NumberOfPlayers;
    UE_LOG(Pro4, Warning, TEXT("Reached Player"));

    if (NumberOfPlayers >= 3 && !isGameStart)
    {
        UE_LOG(Pro4, Warning, TEXT("Start Timer..."));
        GetWorldTimerManager().SetTimer(GameStartTimer, this, &AInGameMode::CountingTheSeconds, 1.0f, true, 5.0f);
        isGameStart = true;
    }

}

/* 플레이어가 세션에서 나갔을 때 실행되는 함수 */
void AInGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;

    if (isGameStart && NumberOfPlayers == 1)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Game Over"));
    }

}

/* 게임을 시작하는 함수 */
void AInGameMode::StartGame()
{
    UNecrophobiaGameInstance* GameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->StartSession();

    UEngine* Engine = GameInstance->GetEngine();
    if (Engine == nullptr) return;
    Engine->AddOnScreenDebugMessage(0, 10.0f, FColor::Red, TEXT("Game Start!"));

    TArray<FVector> SpawnArray;
    SpawnArray.Add(FVector(-45470.0f, 19260.0f, -840.0f));
    SpawnArray.Add(FVector(0.0f, 0.0f, 100.0f));
    SpawnArray.Add(FVector(100.0f, 0.0f, 100.0f));
    SpawnArray.Add(FVector(0.0f, 100.0f, 100.0f));
    SpawnArray.Add(FVector(100.0f, 100.0f, 100.0f));

    InGameState->SpawnPlayerToStartLocation(SpawnArray);
}

/* 충분한 인원이 모이고 시간초를 세는 함수 */
void AInGameMode::CountingTheSeconds()
{
    UE_LOG(Pro4, Warning, TEXT("Timer : %d"), CountSeconds);
    if (--CountSeconds <= 0)
    {
        StartGame();
        GetWorldTimerManager().ClearTimer(GameStartTimer);
    }
}

void AInGameMode::SpawnBossZombie()
{
    FVector SpawnLocation = FVector(-47632.0f, 19246.0f, 40.0f);
    FRotator SpawnRotation = FRotator(0.0f);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    GetWorld()->SpawnActor<APro4Boss>(APro4Boss::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

    InGameState->SetIsBossSpawn(true);
    InGameState->SetIsTimeToSpawnBoss(false);
}