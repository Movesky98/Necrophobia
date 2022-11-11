// Fill out your copyright notice in the Description page of Project Settings.

#include "AGrenade.h"
#include "../Pro4Character.h"
#include "../Pro4Zombie.h"

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

	BoxMesh->SetRelativeScale3D(FVector(3.0f));

	/* 사운드 세팅 */
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Grenade(TEXT("/Game/Sounds/Grenade-Explosion_Cue"));
	if (SC_Grenade.Succeeded())
	{
		ExplosionSound = SC_Grenade.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Smoke(TEXT("/Game/SmokeFlashGrenades/GrenadesSounds/SmokeBurstCue"));
	if (SC_Smoke.Succeeded())
	{
		SmokeSound = SC_Smoke.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Flash(TEXT("/Game/Sounds/FlashBang_Cue"));
	if (SC_Flash.Succeeded())
	{
		FlashSound = SC_Flash.Object;
	}

	AC = CreateDefaultSubobject<UAudioComponent>(TEXT("AC"));
	AC->bAutoActivate = false;
	AC->SetupAttachment(BoxMesh);

	/* 파티클 세팅 */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("/Game/Impacts/Particles/Explosion/Explosion_2/P_Explosion_2_CheapTrails"));
	if (P_Explosion.Succeeded())
	{
		P_GrenadeParticle = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Smoke(TEXT("/Game/SmokeFlashGrenades/SmokeParticles/PS_SmokeCPU"));
	if (P_Smoke.Succeeded())
	{
		P_SmokeParticle = P_Smoke.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Flash(TEXT("/Game/SmokeFlashGrenades/SmokeParticles/PS_Flashbang"));
	if (P_Flash.Succeeded())
	{
		P_FlashParticle = P_Flash.Object;
	}

	uint32 RandomItemNum = FMath::RandRange(0, static_cast<int32>(GrenadeType::MAX) - 1);
	RandomSpawn(RandomItemNum);

	BoxMesh->SetRelativeScale3D(FVector(2.0f));

	NameWidget->InitWidget();
	WBP_NameWidget = Cast<UItemNameWidget>(NameWidget->GetUserWidgetObject());
}

/* 아이템이 월드에 생성되었을 때, 실행되는 함수 */
void AAGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SM_GrenadeItem, TemporaryName, 1, CurrentParticle);
	}
}

/* Server -> Client들에게 생성된 투척무기 정보를 뿌려주는 함수 */
void AAGrenade::NetMulticast_SetUp_Implementation(UStaticMesh* SM_Grenade, const FString& _ItemName, uint16 _ItemNum, UParticleSystem* ParticleSystem)
{
	if (WBP_NameWidget == nullptr)
	{
		return;
	}

	GrenadeParticle->SetTemplate(ParticleSystem);
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
		CurrentParticle = P_GrenadeParticle;
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

		CurrentParticle = P_FlashParticle;
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

		CurrentParticle = P_SmokeParticle;
		TemporaryName = "Smoke";
	}
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("Grenade : Random Setting ERROR"));
		break;
	}
}

/* 투척무기가 폭발했을 때 실행되는 함수 */
void AAGrenade::SetGrenadeExplosion()
{
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	float ParticleTime = 0.0f;
	SetStateToExplosion();

	if (ItemName == "Grenade")
	{
		ParticleTime = 1.5f;

		TArray<FHitResult> OutHits;

		FVector ExplosionLocation = GetActorLocation();
		FName ProfileName = "Grenade";
		FCollisionShape GrenadeColSphere = FCollisionShape::MakeSphere(500.0f);
		FCollisionQueryParams GrenadeColParams;

		DrawDebugSphere(GetWorld(), ExplosionLocation, GrenadeColSphere.GetSphereRadius(), 30, FColor::Green, true, 5.0f);

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

					PlayerCharacter->GetDamaged(40.0f, GetOwner());
				}
				else if (Hit.GetActor()->ActorHasTag("Zombie"))
				{
					DrawDebugSolidBox(GetWorld(), Hit.GetActor()->GetActorLocation(), FVector(50.0f), FColor::Red, true, -1);
					APro4Zombie* Zombie = Cast<APro4Zombie>(Hit.GetActor());

					Zombie->ZombieGetDamaged(40.0f, GetOwner());
				}
			}
		}
	}
	else if (ItemName == "Smoke")
	{
		ParticleTime = 15.0f;
	}
	else if (ItemName == "Flash")
	{
		ParticleTime = 2.0f;
		TArray<FHitResult> OutHits;

		FVector ExplosionLocation = GetActorLocation();
		FName ProfileName = "Grenade";
		FCollisionShape GrenadeColSphere = FCollisionShape::MakeSphere(800.0f);
		FCollisionQueryParams GrenadeColParams;

		DrawDebugSphere(GetWorld(), ExplosionLocation, GrenadeColSphere.GetSphereRadius(), 30, FColor::Green, true, 5.0f);

		bool bIsHit = GetWorld()->SweepMultiByProfile(OutHits, ExplosionLocation, ExplosionLocation, FQuat::Identity, ProfileName, GrenadeColSphere);

		if (bIsHit)
		{
			for (auto& Hit : OutHits)
			{
				if (Hit.GetActor()->ActorHasTag("Player"))
				{
					DrawDebugSolidBox(GetWorld(), Hit.GetActor()->GetActorLocation(), FVector(50.0f), FColor::Red, true, -1);
					APro4Character* PlayerCharacter = Cast<APro4Character>(Hit.GetActor());

					PlayerCharacter->FlashBangExplosion();
				}
			}
		}
	}

	PlayGrenadeSound();

	GetWorldTimerManager().SetTimer(SetExplosionTimer, this, &AAGrenade::GrenadeExplosion, ParticleTime);
}

/* 수류탄 폭발 파티클이 끝나고 실행되는 함수 */
void AAGrenade::GrenadeExplosion()
{
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	Destroy();
}

void AAGrenade::ThrowGrenade_Implementation(const FString& GrenadeType, UStaticMesh* GrenadeMesh)
{
	if (WBP_NameWidget == nullptr)
	{
		return;
	}

	if (GrenadeType == "Grenade")
	{
		GrenadeParticle->SetTemplate(P_GrenadeParticle);
		CurrentSound = ExplosionSound;
	}
	else if (GrenadeType == "Smoke")
	{
		GrenadeParticle->SetTemplate(P_SmokeParticle);
		CurrentSound = SmokeSound;
	}
	else if (GrenadeType == "Flash")
	{
		GrenadeParticle->SetTemplate(P_FlashParticle);
		CurrentSound = FlashSound;
	}

	AC->SetSound(Cast<USoundBase>(CurrentSound));
	BoxMesh->SetStaticMesh(GrenadeMesh);
	ItemName = GrenadeType;
	WBP_NameWidget->SetItemName(ItemName);
	ItemNum = 1;
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

void AAGrenade::PlayGrenadeSound_Implementation()
{
	AC->SetSound(Cast<USoundBase>(CurrentSound));
	AC->Play();
}

void AAGrenade::SetStateToExplosion_Implementation()
{
	BoxMesh->SetVisibility(false);
	GrenadeProjectile->bSimulationEnabled = false;
	GrenadeParticle->ToggleActive();
}

/* 아이템에서 서버와 클라이언트에 복제되는 변수들을 설정하는 함수 */
void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAGrenade, ItemName);
	DOREPLIFETIME(AAGrenade, ItemNum);
}