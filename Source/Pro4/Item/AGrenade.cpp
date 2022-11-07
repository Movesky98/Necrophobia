// Fill out your copyright notice in the Description page of Project Settings.

#include "AGrenade.h"
#include "../Pro4Character.h"

#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"


AAGrenade::AAGrenade()
{
	ItemType = BaseItemType::Grenade;
	GrenadeProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeProjectile"));
	GrenadeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GrenadeParticle"));
	
	GrenadeProjectile->SetUpdatedComponent(BoxMesh);
	GrenadeProjectile->InitialSpeed = 0.0f;
	GrenadeProjectile->MaxSpeed = 1500.0f;
	GrenadeProjectile->bRotationFollowsVelocity = true;
	GrenadeProjectile->bShouldBounce = true;
	GrenadeProjectile->Bounciness = 0.3f;
	GrenadeProjectile->bSimulationEnabled = false;

	GrenadeParticle->SetupAttachment(BoxMesh);
	GrenadeParticle->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<USoundCue>ThrowSound(TEXT("/Game/StarterContent/Audio/ThrowFires"));
	if (ThrowSound.Succeeded())
	{
		SC = ThrowSound.Object;
	}
	AC = CreateDefaultSubobject<UAudioComponent>(TEXT("AC"));
	AC->bAutoActivate = false;
	AC->SetupAttachment(BoxMesh);

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(GrenadeType::MAX) - 1);
	RandomSpawn(RandomItemNum);

	BoxMesh->SetRelativeScale3D(FVector(2.0f));

	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_GrenadeExplosion(TEXT("/Game/Impacts/Particles/Explosion/Explosion_2/P_Explosion_2_CheapTrails"));
	if (P_GrenadeExplosion.Succeeded())
	{
		UE_LOG(Pro4, Warning, TEXT("Grenade Particle has Succeeded"));
		GrenadeParticle->SetTemplate(P_GrenadeExplosion.Object);
		
	}
}

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AAGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SM_GrenadeItem, TemporaryName, 1);
	}
}

/* Server -> Client들에게 생성된 투척무기 정보를 뿌려주는 함수 */
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

/* 아이템의 이름을 가진 UI를 보여주는 함수 */
void AAGrenade::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* 아이템과 겹치기 시작하는 액터가 있을 때 실행되는 함수*/
void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* 아이템과 겹치고 있던 액터가 벗어날 때 실행되는 함수*/
void AAGrenade::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* 투척무기 종류 중, 랜덤으로 투척무기의 정보를 설정하는 함수 */
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
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	TArray<FHitResult> OutHits;

	FVector ExplosionLocation = GetActorLocation();
	FName ProfileName = "Grenade";
	FCollisionShape GrenadeColSphere = FCollisionShape::MakeSphere(500.0f);
	FCollisionQueryParams GrenadeColParams;

	DrawDebugSphere(GetWorld(), ExplosionLocation, GrenadeColSphere.GetSphereRadius(), 30, FColor::Green, true, 5.0f);

	BoxMesh->SetVisibility(false);
	GrenadeProjectile->bSimulationEnabled = false;
	GrenadeParticle->ToggleActive();

	bool bIsHit = GetWorld()->SweepMultiByProfile(OutHits, ExplosionLocation, ExplosionLocation, FQuat::Identity, ProfileName, GrenadeColSphere);

	if (bIsHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, TEXT("Grenade is Expluded!"));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, FString::FromInt(OutHits.Num()));

		for (auto& Hit : OutHits)
		{
			if (Hit.GetActor()->ActorHasTag("Player"))
			{
				DrawDebugSolidBox(GetWorld(), Hit.GetActor()->GetActorLocation(), FVector(50.0f), FColor::Red, true, -1);
				APro4Character* PlayerCharacter = Cast<APro4Character>(Hit.GetActor());

				PlayerCharacter->GetDamaged(40.0f);
			}

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, Hit.GetActor()->GetName());
		}
	}
	AC->SetSound(Cast<USoundBase>(SC));
	AC->Play();
	GetWorldTimerManager().SetTimer(SetExplosionTimer, this, &AAGrenade::GrenadeExplosion, 1.5f);
}

/* 수류탄 폭발 파티클이 끝나고 실행되는 함수 */
void AAGrenade::GrenadeExplosion()
{
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	Destroy();
}

/* 수류탄을 던졌을 때, 날아가도록 하는 함수 */
void AAGrenade::SetSimulatePhysics(const FVector& ThrowDirection)
{
	BoxMesh->SetRelativeScale3D(FVector(10.0f));
	WBP_NameWidget->SetVisibility(ESlateVisibility::Hidden);
	GrenadeProjectile->bSimulationEnabled = true;
	GrenadeProjectile->Velocity = ThrowDirection * GrenadeProjectile->MaxSpeed;

	GetWorldTimerManager().SetTimer(SetExplosionTimer, this, &AAGrenade::SetGrenadeExplosion, 5.0f);
}

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAGrenade, ItemName);
	DOREPLIFETIME(AAGrenade, ItemNum);
}