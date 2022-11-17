// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMode.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "Pro4Boss.h"
#include "NecrophobiaGameInstance.h"
#include "InGameState.h"
#include "InGamePlayerState.h"
#include "D_EncroachField.h"
#include "Item/AItemSpawner.h"
#include "UserInterface/PlayerMenu.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AInGameMode::AInGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    // set default pawn class to our Blueprinted character

    static ConstructorHelpers::FClassFinder<APro4PlayerController> BP_PlayerController(TEXT("/Game/Blueprints/BP_PlayerController"));
    if (BP_PlayerController.Succeeded())
    {
        PlayerControllerClass = BP_PlayerController.Class;
    }
    else
    {
        PlayerControllerClass = APro4PlayerController::StaticClass();
    }
    
    DefaultPawnClass = APro4Character::StaticClass();
    UE_LOG(Pro4, Warning, TEXT("Set PawnClass : APro4Character"));
    GameStateClass = AInGameState::StaticClass();
    PlayerStateClass = AInGamePlayerState::StaticClass();

    // 게임에 입장한 플레이어 수 (총 플레이어 수, 서버 제외)
    NumberOfPlayers = -1;
    isGameStart = false;
    CountSeconds = 30;
    Time = 0;
}

/* 월드에 생성되었을 때 실행되는 함수 */
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

    // 월드에서 배치된 아이템 스포너들을 받아서 ItemSpawnerArray에 저장
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAItemSpawner::StaticClass(), ItemSpawnerArray);
    
    FVector SpawnLocation = FVector(-47769.0f, 30816.0f, 92.0f);
    FRotator SpawnRotation = FRotator(0.0f);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    GetWorld()->SpawnActor<AD_EncroachField>(AD_EncroachField::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
}

/* 매 프레임마다 실행되는 함수 */
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

        // 아이템 생성 시기에, 아이템 스포너에서 생성.
        if (InGameState->GetIsTimeToSpawnItem())
        {
            InGameState->SetIsTimeToSpawnItem(false);

            for (AActor* ItemSpawnerActor : ItemSpawnerArray)
            {
                AAItemSpawner* ItemSpawner = Cast<AAItemSpawner>(ItemSpawnerActor);
                
                ItemSpawner->Server_SpawnItem();
            }
            
        }
    }
}

/* 플레이어가 세션에 입장하고 로그인 한 후 실행되는 함수 */
void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ++NumberOfPlayers;
    UE_LOG(Pro4, Warning, TEXT("Reached Player"));

    if (NumberOfPlayers >= 2 && !isGameStart)
    {
        UE_LOG(Pro4, Warning, TEXT("Start Timer..."));
        GetWorldTimerManager().SetTimer(GameStartTimer, this, &AInGameMode::CountingTheSeconds, 1.0f, true);
        isGameStart = true;
    }
}

/* 플레이어가 세션에서 나갔을 때 실행되는 함수 */
void AInGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;

    // 게임이 시작되고 플레이어가 한명만 남았다면 게임 끝.
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
    SpawnArray.Add(FVector(0.0f, 0.0f, -1000.0f));
    SpawnArray.Add(FVector(-45470.0f, 19260.0f, -840.0f));
    SpawnArray.Add(FVector(-55546, 28496, 910));
    SpawnArray.Add(FVector(-53433.0f, 36765.0f, 1110.0f));
    SpawnArray.Add(FVector(-50113.0f, 30207.0f, 363.0f));

    InGameState->SpawnPlayerToStartLocation(SpawnArray);
    InGameState->SetIsTimeToSpawnItem(true);
    InGameState->SetEscapePlayer(0);
    InGameState->SetSurvivePlayer(NumberOfPlayers);
    InGameState->SetTotalPlayer(NumberOfPlayers);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Total Player Num : %d"), NumberOfPlayers));

    int32 NumberOfSpawner = ItemSpawnerArray.Num();

    if (NumberOfSpawner > 0)
    {
        // 아이템 스포너에 백신 생성
        for (int i = 0; i < 3; i++)
        {
            int32 Rand = FMath::RandRange(0, NumberOfSpawner);
            AAItemSpawner* ItemSpawner = Cast<AAItemSpawner>(ItemSpawnerArray[Rand]);

            ItemSpawner->SpawnVaccine();
        }
    }
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

/* 서버가 보스를 생성하는 함수 */
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