// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4PlayerController.h"
#include "Pro4Character.h"
#include "InGameState.h"
#include "NecrophobiaGameInstance.h"
#include "UserInterface/PlayerMenu.h"
#include "Item/AArmor.h"
#include "Item/AWeapon.h"
#include "Item/AGrenade.h"

APro4PlayerController::APro4PlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APro4PlayerController::BeginPlay()
{
	Super::BeginPlay();
	InGameState = GetWorld()->GetGameState<AInGameState>();
	UNecrophobiaGameInstance* InGameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());

	if (InGameInstance != nullptr)
	{
		this->PlayerMenu = InGameInstance->PlayerMenu;
	}

}

void APro4PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 시간 차이가 있음 수정 필요.
	Time += DeltaTime;
	if (Time >= 1.0f) 
	{
		Time = 0.0f;
		UpdatePlayerTimeState();
	}
}

void APro4PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

/* 인게임 내 시간을 출력해주는 함수. */
void APro4PlayerController::UpdatePlayerTimeState()
{
	if (IsLocalPlayerController())
	{
		PlayerMenu->SetTimeText(InGameState->GetInGameMinutes(), InGameState->GetInGameSeconds());

		if (InGameState->GetIsNight())
		{
			PlayerMenu->SetImage(PlayerMenu->Night);
		}
		else
		{
			PlayerMenu->SetImage(PlayerMenu->Day);
		}
	}
}

void APro4PlayerController::SpawnArmorOnServer_Implementation(FVector Location, USkeletalMesh* _ArmorMesh, const FString& _ArmorName, float _AP)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		FRotator Rotation;

		AAArmor* DropItem = World->SpawnActor<AAArmor>(AAArmor::StaticClass(), Location, Rotation, SpawnParams);
	}
}