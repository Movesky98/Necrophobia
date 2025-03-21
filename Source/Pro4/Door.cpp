// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorStaticMesh"));
	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	RootComponent = DoorStaticMesh;
	DoorCollision->SetupAttachment(DoorStaticMesh);

	DoorStaticMesh->SetIsReplicated(true);

	DoorCollision->SetCollisionProfileName(TEXT("Door"));

	Tags.Add("Door");

	SetCanBeDamaged(false);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어가 문을 열려고 시도하고 문이 닫혀있는 경우
	if (bIsPlayerOpen && !bIsOpen)
	{
		OpenDoor(DeltaTime);
	}
	else if(bIsPlayerOpen && bIsOpen)
	{
		CloseDoor(DeltaTime);
	}
}

/* 문이 열릴 때 실행되는 함수 */
void ADoor::OpenDoor(float DeltaTime)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += DeltaTime * 100;

	if (NewRotation.Yaw >= 120.0f)
	{
		bIsOpen = true;
		bIsPlayerOpen = false;
		return;
	}
	
	SetActorRotation(NewRotation);
}

/* 열린 문이 다시 닫힐 때 실행되는 함수 */
void ADoor::CloseDoor(float DeltaTime)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw -= DeltaTime * 100;

	if (NewRotation.Yaw <= 0.0f)
	{
		bIsOpen = false;
		bIsPlayerOpen = false;
		return;
	}

	SetActorRotation(NewRotation);
}

/* 플레이어 캐릭터가 문에 다가왔을 때 실행되는 함수 */
void ADoor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SetCanPlayerOpenDoor(true);

		UE_LOG(Pro4, Warning, TEXT("The player is at the door."));
	}
}

/* 플레이어 캐릭터가 문으로부터 멀어질 때 실행되는 함수 */
void ADoor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SetCanPlayerOpenDoor(false);

		UE_LOG(Pro4, Warning, TEXT("Player is out of the door."));
	}
}