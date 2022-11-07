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

/* �������� ���忡 �����Ǿ��� ��, ����Ǵ� �Լ� */
void AAGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsServer())
	{
		NetMulticast_SetUp(SM_GrenadeItem, TemporaryName, 1);
	}
}

/* Server -> Client�鿡�� ������ ��ô���� ������ �ѷ��ִ� �Լ� */
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

/* �������� �̸��� ���� UI�� �����ִ� �Լ� */
void AAGrenade::ViewItemName()
{
	bIsObservable = !bIsObservable;
	WBP_NameWidget->ToggleVisibility();
}

/* �����۰� ��ġ�� �����ϴ� ���Ͱ� ���� �� ����Ǵ� �Լ�*/
void AAGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();

		UE_LOG(Pro4, Log, TEXT("Player is overlapping."));
	}
}

/* �����۰� ��ġ�� �ִ� ���Ͱ� ��� �� ����Ǵ� �Լ�*/
void AAGrenade::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ViewItemName();


		UE_LOG(Pro4, Log, TEXT("Player Overlap End."));
	}
}

/* ��ô���� ���� ��, �������� ��ô������ ������ �����ϴ� �Լ� */
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

/* ����ź�� �������� �� ����Ǵ� �Լ� */
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

/* ����ź ���� ��ƼŬ�� ������ ����Ǵ� �Լ� */
void AAGrenade::GrenadeExplosion()
{
	GetWorldTimerManager().ClearTimer(SetExplosionTimer);
	Destroy();
}

/* ����ź�� ������ ��, ���ư����� �ϴ� �Լ� */
void AAGrenade::SetSimulatePhysics(const FVector& ThrowDirection)
{
	BoxMesh->SetRelativeScale3D(FVector(10.0f));
	WBP_NameWidget->SetVisibility(ESlateVisibility::Hidden);
	GrenadeProjectile->bSimulationEnabled = true;
	GrenadeProjectile->Velocity = ThrowDirection * GrenadeProjectile->MaxSpeed;

	GetWorldTimerManager().SetTimer(SetExplosionTimer, this, &AAGrenade::SetGrenadeExplosion, 5.0f);
}

/* �����ۿ��� ������ Ŭ���̾�Ʈ�� �����Ǵ� �������� �����ϴ� �Լ� */
void AAGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAGrenade, ItemName);
	DOREPLIFETIME(AAGrenade, ItemNum);
}