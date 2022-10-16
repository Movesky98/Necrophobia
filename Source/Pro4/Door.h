// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PRO4_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	bool GetIsOpen()
	{
		return bIsOpen;
	}

	void SetIsOpen(bool _bIsOpen)
	{
		bIsOpen = _bIsOpen;
	}

	bool GetCanPlayerOpenDoor()
	{
		return bCanPlayerOpen;
	}

	void SetCanPlayerOpenDoor(bool _bCanPlayerOpen)
	{
		bCanPlayerOpen = _bCanPlayerOpen;
	}
	
	bool GetIsPlayerOpen()
	{
		return bIsPlayerOpen;
	}

	void SetIsPlayerOpen(bool _bIsPlayerOpen)
	{
		bIsPlayerOpen = _bIsPlayerOpen;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Door)
	UStaticMeshComponent* DoorStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Door)
	class UBoxComponent* DoorCollision;
	
	bool bIsOpen = false;
	bool bCanPlayerOpen = false;
	bool bIsPlayerOpen = false;
};
