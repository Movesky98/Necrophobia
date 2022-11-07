// Fill out your copyright notice in the Description page of Project Settings.


#include "D_EncroachField.h"
#include "Pro4Character.h"

#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

/* 기초 설정 */
AD_EncroachField::AD_EncroachField()
{
	bReplicates = true;
	bNetLoadOnClient = true;

	EncroachPivotComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EncroachPoint"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>M_Polluted(TEXT("/Game/M_Polluted_Decal_"));
	if (M_Polluted.Succeeded())
	{
		UMaterialInterface* DecalMaterial = M_Polluted.Object;
		SetDecalMaterial(DecalMaterial);
	}

	RootComponent = GetDecal();

	EncroachPivotComponent->SetupAttachment(RootComponent);
	EncroachPivotComponent->InitSphereRadius(200.0f);
	EncroachPivotComponent->SetCollisionProfileName(TEXT("EncroachField"));

	Tags.Add("Encroach");
}

// Called when the game starts or when spawned
void AD_EncroachField::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float SearchRadius = 50.0f;
	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
}

/* EncroachField와 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AD_EncroachField::NotifyActorBeginOverlap(AActor* Act)
{
	UE_LOG(Pro4, Log, TEXT("Its in the encroaching field"));

	if (Act->ActorHasTag("Player"))
	{
		APro4Character* PlayerCharacter = Cast<APro4Character>(Act);
		PlayerCharacter->Encroached();
		PlayerCharacter->StartEncroachTimer();
	}
}

/* EncroachField와 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AD_EncroachField::NotifyActorEndOverlap(AActor* Act)
{
	UE_LOG(Pro4, Log, TEXT("Its out of the encroaching field"));
	if (Act->ActorHasTag("Player"))
	{
		APro4Character* PlayerCharacter = Cast<APro4Character>(Act);
		PlayerCharacter->UnEncroached();
		PlayerCharacter->StopEncroachTimer();
	}
}