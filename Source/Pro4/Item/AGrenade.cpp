// Fill out your copyright notice in the Description page of Project Settings.

#include "AGrenade.h"
#include "../Pro4Character.h"

#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"


AAGrenade::AAGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = BaseItemType::Grenade;
	GrenadeProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeProjectile"));
	
	GrenadeProjectile->SetUpdatedComponent(BoxMesh);
	GrenadeProjectile->InitialSpeed = 0.0f;
	GrenadeProjectile->MaxSpeed = 1500.0f;
	GrenadeProjectile->bRotationFollowsVelocity = true;
	GrenadeProjectile->bShouldBounce = true;
	GrenadeProjectile->Bounciness = 0.3f;
	GrenadeProjectile->bSimulationEnabled = false;

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(GrenadeType::MAX) - 1);
	RandomSpawn(RandomItemNum);

	BoxMesh->SetRelativeScale3D(FVector(2.0f));

	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

void AAGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SM_GrenadeItem, TemporaryName, 1);
	}
}

void AAGrenade::NetMulticast_SetUp_Implementation(UStaticMesh* SM_Grenade, const FString& _ItemName, uint16 _ItemNum)
{
	if (WBP_NameWidget == nullptr)
	{
		return;
	}

	BoxMesh->SetStaticMesh(SM_Grenade);
	ItemName = _ItemName;
	WBP_NameWidget->SetItemName(ItemName);
	ItemNum = _ItemNum;
}

void AAGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ItemName Draw
	// DrawDebugString(GetWorld(), FVector(0, 0, 100), ItemName, this, FColor::Green, DeltaTime);
}

void AAGrenade::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

void AAGrenade::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

void AAGrenade::RandomSpawn(int32 Random)
{
	CurrentGrenade = static_cast<GrenadeType>(Random);

	switch (CurrentGrenade)
	{
	case AAGrenade::GrenadeType::Grenade:
	{
		UE_LOG(Pro4, Log, TEXT("Grenade is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/Weapon/Granade/granade"));
		if (SM_Grenade.Succeeded())
		{
			SM_GrenadeItem = SM_Grenade.Object;
		}
		
		TemporaryName = "Grenade";
	}
		break;
	case AAGrenade::GrenadeType::Flash:
	{
		UE_LOG(Pro4, Log, TEXT("Flash is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/Weapon/Granade/flashbang"));
		if (SM_Flash.Succeeded())
		{
			SM_GrenadeItem = SM_Flash.Object;
		}

		TemporaryName = "Flash";
	}
		break;
	case AAGrenade::GrenadeType::Smoke:
	{
		UE_LOG(Pro4, Log, TEXT("Smoke is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/Weapon/Granade/smoke"));
		if (SM_Smoke.Succeeded())
		{
			SM_GrenadeItem = SM_Smoke.Object;
		}

		TemporaryName = "Smoke";
	}
		break;
	case AAGrenade::GrenadeType::Molotov:
	{
		UE_LOG(Pro4, Log, TEXT("Molotov is spawned."));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Molotov(TEXT("/Game/Weapon/Granade/firebomb"));
		if (SM_Molotov.Succeeded())
		{
			SM_GrenadeItem = SM_Molotov.Object;
		}

		TemporaryName = "Molotov";
	}
		break;
	}
}

/* 수류탄이 폭발했을 때 실행되는 함수 */
void AAGrenade::SetGrenadeExplosion()
{
	TArray<FHitResult> OutHits;

	FVector ExplosionLocation = GetActorLocation();

	FCollisionShape GrenadeColSphere = FCollisionShape::MakeSphere(500.0f);
	DrawDebugSphere(GetWorld(), ExplosionLocation, GrenadeColSphere.GetSphereRadius(), 30, FColor::Green, true, 5.0f);

	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, ExplosionLocation, ExplosionLocation, FQuat::Identity, ECC_Visibility, GrenadeColSphere);

	if (bIsHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, TEXT("Grenade is Expluded!"));
		
		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor()->ActorHasTag("Player"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, Hit.GetActor()->GetName());
				
				APro4Character* PlayerCharacter = Cast<APro4Character>(Hit.GetActor());

				PlayerCharacter->GetDamaged(40.0f);
			}
		}
	}
	
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	Destroy();
}

void AAGrenade::GrenadeExplosion(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AAGrenade::SetSimulatePhysics(const FVector& ThrowDirection)
{
	BoxMesh->SetRelativeScale3D(FVector(10.0f));
	GrenadeProjectile->bSimulationEnabled = true;
	GrenadeProjectile->Velocity = ThrowDirection * GrenadeProjectile->MaxSpeed;

	GetWorldTimerManager().SetTimer(SetExplosionTimer, this, &AAGrenade::SetGrenadeExplosion, 1.0f, false, 5.0f);
}

void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAGrenade, ItemName);
	DOREPLIFETIME(AAGrenade, ItemNum);
}