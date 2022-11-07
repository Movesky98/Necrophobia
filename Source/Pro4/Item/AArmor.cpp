// Fill out your copyright notice in the Description page of Project Settings.

#include "AArmor.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

AAArmor::AAArmor()
{
	ItemType = BaseItemType::Armor;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(ArmorType::MAX) - 1);
	RandomSpawn(RandomItemNum);

	/* Widget Component에서 아이템의 이름을 가진 Widget을 가져옴 */
	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AAArmor::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SK_ArmorItem, TemporaryName, 1);
	}
}

/* Server -> Client들에게 생성된 방어구 정보를 뿌려주는 함수 */
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
}

/* 아이템의 이름을 가진 UI를 보여주는 함수 */
void AAArmor::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* 아이템과 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AAArmor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* 아이템과 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AAArmor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* 방어구 종류 중, 랜덤으로 방어구의 정보를 설정하는 함수 */
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

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAArmor, Current_AP);
	DOREPLIFETIME(AAArmor, ItemName);
	DOREPLIFETIME(AAArmor, ItemNum);
}