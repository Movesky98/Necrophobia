// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pro4GameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "NecrophobiaGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

#include "TimerManager.h"

/*
#include <Steamworks/Steamv153/sdk/public/steam/steam_api.h>
 스팀 라이브러리를 명시적으로 연결함
#pragma comment(lib, "ThirdParty/Steamworks/Steamv153/sdk/redistributable_bin/win64/steam_api64.lib")
*/


APro4GameMode::APro4GameMode()
{
   // set default pawn class to our Blueprinted character
   DefaultPawnClass = APro4Character::StaticClass();
   PlayerControllerClass = APro4PlayerController::StaticClass();
}
void APro4GameMode::BeginPlay()

{
   Super::BeginPlay();
   /*
   if (!SteamAPI_RestartAppIfNecessary(480)) // 앱 ID로 대체 
   {
      false가 나올 경우, Steam 클라이언트에서 게임을 실행한 것으로 인식함 
      UE_LOG(Pro4, Error, TEXT("SteamAPI_RestartAppIfNecessary Successed"));
   }

   if(!SteamAPI_Init())
   {
      UE_LOG(Pro4, Error, TEXT("SteamAPI_Init failed"));
   }
   else
   {
      UE_LOG(Pro4, Error, TEXT("SteamAPI_Init Successed"));
   }

   Encoding 형식 UTF-8로 변경 테스트
   */
}

/* 플레이어가 세션에 입장하고 로그인 한 후 실행되는 함수 */
void APro4GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ++NumberOfPlayers;
    UE_LOG(Pro4, Warning, TEXT("Reached Player"));

    if (NumberOfPlayers >= 3 && !isSetStartTimer)
    {
        UE_LOG(Pro4, Warning, TEXT("Start Timer..."));
        GetWorldTimerManager().SetTimer(GameStartTimer, this, &APro4GameMode::CountingTheSeconds, 1.0f, true, 30.0f);
        isSetStartTimer = true;
    }

}
/* 플레이어가 세션에서 나갔을 때 실행되는 함수 */
void APro4GameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

}

/* 게임을 시작하는 함수 */
void APro4GameMode::StartGame()
{
    UNecrophobiaGameInstance* GameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->StartSession();

    UEngine* Engine = GameInstance->GetEngine();
    if (Engine == nullptr) return;
    Engine->AddOnScreenDebugMessage(0, 10.0f, FColor::Red, TEXT("Game Start!"));
}

/* 충분한 인원이 모이고 시간초를 세는 함수 */
void APro4GameMode::CountingTheSeconds()
{
    UE_LOG(Pro4, Warning, TEXT("Timer : %d"), CountSeconds);
    if (--CountSeconds <= 0)
    {
        GetWorldTimerManager().ClearTimer(GameStartTimer);
        StartGame();
    }
}