// Fill out your copyright notice in the Description page of Project Settings.

#include "AArmor.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAArmor::AAArmor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Armor;
	bReplicates = true;
	bNetLoadOnClient = true;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(ArmorType::MAX) - 1);
	RandomSpawn(RandomItemNum);
}

void AAArmor::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{
		BoxMesh->SetStaticMesh(SM_ArmorItem);
		ItemName = TemporaryName;
		ItemNum = 1;
	}
}

void AAArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	DrawDebugString(GetWorld(), FVector(0, 0, 100), ItemName, this, FColor::Green, DeltaTime);
}

void AAArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(Pro4, Log, TEXT("Armor is overlapping."));
}

void AAArmor::RandomSpawn(int32 Random)
{
	CurrentArmor = static_cast<ArmorType>(Random);

	switch (CurrentArmor)
	{
	case AAArmor::ArmorType::Helmet:
	{
		UE_LOG(Pro4, Log, TEXT("Helmet(Armor) is spawned."));
		
		TemporaryName = "Helmet";
	}
		break;
	case AAArmor::ArmorType::Flak_Jacket:
	{
		UE_LOG(Pro4, Log, TEXT("Flak_Jacket(Armor) is spawned."));
		TemporaryName = "Flak_Jacket";
	}
		break;
	default:
		UE_LOG(Pro4, Log, TEXT("Armor Spawn ERROR"));
		break;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Armor(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_Y"));
	if (SM_Armor.Succeeded())
	{
		SM_ArmorItem = SM_Armor.Object;
	}
}

void AAArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAArmor, ItemName);
	DOREPLIFETIME(AAArmor, ItemNum);
}