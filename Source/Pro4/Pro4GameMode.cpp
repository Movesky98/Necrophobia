// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pro4GameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include <Steamworks/Steamv153/sdk/public/steam/steam_api.h>
// 스팀 라이브러리를 명시적으로 연결함
#pragma comment(lib, "ThirdParty/Steamworks/Steamv153/sdk/redistributable_bin/win64/steam_api64.lib")



APro4GameMode::APro4GameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = APro4Character::StaticClass();
	PlayerControllerClass = APro4PlayerController::StaticClass();
}
void APro4GameMode::BeginPlay()

{
	Super::BeginPlay();
	//if (!SteamAPI_RestartAppIfNecessary(480)) // 앱 ID로 대체 
	//{
		// false가 나올 경우, Steam 클라이언트에서 게임을 실행한 것으로 인식함 
	//	UE_LOG(Pro4, Error, TEXT("SteamAPI_RestartAppIfNecessary Successed"));
	//}

	if(!SteamAPI_Init())
	{
		UE_LOG(Pro4, Error, TEXT("SteamAPI_Init failed"));
	}
	else
	{
		UE_LOG(Pro4, Error, TEXT("SteamAPI_Init Successed"));
	}
}
