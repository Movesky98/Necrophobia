// Fill out your copyright notice in the Description page of Project Settings.

#include "AArmor.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAArmor::AAArmor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Armor;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(ArmorType::MAX) - 1);
	RandomSpawn(RandomItemNum);

	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

void AAArmor::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SK_ArmorItem, TemporaryName, 1);
	}
}

void AAArmor::NetMulticast_SetUp_Implementation(USkeletalMesh* SK_Armor, const FString& _ItemName, uint16 _ItemNum)
{
	if (WBP_NameWidget == nullptr)
	{
		return;
	}

	SK_Mesh->SetSkeletalMesh(SK_Armor);
	ItemName = _ItemName;
	WBP_NameWidget->SetItemName(ItemName);
	ItemNum = _ItemNum;

	Current_AP = 50.0f;
	//if (ItemName == "Helmet")
	//{
	//	// HelmetSettings
	//	SK_Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -220.0f));
	//}
	//else
	//{
	//	// Vest Settings
	//	SK_Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -160.0f));
	//}
}

void AAArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	// DrawDebugString(GetWorld(), FVector(0, 0, 100), ItemName, this, FColor::Green, DeltaTime);
}

void AAArmor::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void AAArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void AAArmor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void AAArmor::RandomSpawn(int32 Random)
{
	CurrentArmor = static_cast<ArmorType>(Random);

	switch (CurrentArmor)
	{
	case AAArmor::ArmorType::Helmet:
	{
		UE_LOG(Pro4, Log, TEXT("Helmet(Armor) is spawned."));
		
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Armor(TEXT("/Game/Military/Mesh/Head/SK_Military_Helmet1"));
		if (SK_Armor.Succeeded())
		{
			SK_ArmorItem = SK_Armor.Object;
		}

		TemporaryName = "Helmet";
	}
		break;
	case AAArmor::ArmorType::Flak_Jacket:
	{
		UE_LOG(Pro4, Log, TEXT("Flak_Jacket(Armor) is spawned."));

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Armor(TEXT("/Game/Military/Mesh/SK_Military_Vest1"));
		if (SK_Armor.Succeeded())
		{
			SK_ArmorItem = SK_Armor.Object;
		}

		TemporaryName = "Flak_Jacket";
	}
		break;
	default:
		UE_LOG(Pro4, Log, TEXT("Armor Spawn ERROR"));
		break;
	}
}

void AAArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAArmor, Current_AP);
	DOREPLIFETIME(AAArmor, ItemName);
	DOREPLIFETIME(AAArmor, ItemNum);
}