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

    // ���ӿ� ������ �÷��̾� �� (�� �÷��̾� ��, ���� ����)
    NumberOfPlayers = -1;
    isGameStart = false;
    CountSeconds = 30;
    Time = 0;
}

/* ���忡 �����Ǿ��� �� ����Ǵ� �Լ� */
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

    // ���忡�� ��ġ�� ������ �����ʵ��� �޾Ƽ� ItemSpawnerArray�� ����
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAItemSpawner::StaticClass(), ItemSpawnerArray);
    
    FVector SpawnLocation = FVector(-47769.0f, 30816.0f, 92.0f);
    FRotator SpawnRotation = FRotator(0.0f);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    GetWorld()->SpawnActor<AD_EncroachField>(AD_EncroachField::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
}

/* �� �����Ӹ��� ����Ǵ� �Լ� */
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

        // ������ ���� �ñ⿡, ������ �����ʿ��� ����.
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

/* �÷��̾ ���ǿ� �����ϰ� �α��� �� �� ����Ǵ� �Լ� */
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

/* �÷��̾ ���ǿ��� ������ �� ����Ǵ� �Լ� */
void AInGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;

    // ������ ���۵ǰ� �÷��̾ �Ѹ� ���Ҵٸ� ���� ��.
    if (isGameStart && NumberOfPlayers == 1)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Game Over"));
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
        // ������ �����ʿ� ��� ����
        for (int i = 0; i < 3; i++)
        {
            int32 Rand = FMath::RandRange(0, NumberOfSpawner);
            AAItemSpawner* ItemSpawner = Cast<AAItemSpawner>(ItemSpawnerArray[Rand]);

            ItemSpawner->SpawnVaccine();
        }
    }
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

/* ������ ������ �����ϴ� �Լ� */
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