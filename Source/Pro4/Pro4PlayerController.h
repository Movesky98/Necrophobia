// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/PlayerController.h"
#include "Pro4PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API APro4PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APro4PlayerController();

	void SetPlayerCharacter(class APro4Character* _Character)
	{
		PlayerCharacter = Cast<class APro4Character>(_Character);
	}

	AActor* GetPlayerCharacter()
	{
		return Cast<AActor>(PlayerCharacter);
	}
	/* PlayerCharacter <-> Item Sector */
	UFUNCTION(Server, Reliable)
	void SpawnArmorOnServer(FVector Location, USkeletalMesh* _ArmorMesh, const FString& _ArmorName, float _AP);

protected:

	virtual void OnPossess(APawn* InPawn);

	void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void UpdatePlayerTimeState();
	float Time = 0.0f;

	class APro4Character* PlayerCharacter;

	class AInGameState* InGameState;
	class UPlayerMenu* PlayerMenu;
};
