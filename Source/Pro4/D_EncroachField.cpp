// Fill out your copyright notice in the Description page of Project Settings.


#include "D_EncroachField.h"
#include "Pro4Character.h"

#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

/* ���� ���� */
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
	EncroachPivotComponent->SetIsReplicated(true);
	EncroachPivotComponent->SetCollisionProfileName(TEXT("EncroachField"));

	FieldSize = 1.0f;

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

	GetWorldTimerManager().SetTimer(EncroachFieldTimer, this, &AD_EncroachField::GrowEncroachField, 0.1f, true);
}

/* EncroachField�� ��ġ�� �����ϴ� ���Ͱ� ���� �� ����Ǵ� �Լ�*/
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

/* EncroachField�� ��ġ�� �ִ� ���Ͱ� ��� �� ����Ǵ� �Լ�*/
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

void AD_EncroachField::GrowEncroachField_Implementation()
{
	FieldSize += 0.1f;

	GetDecal()->SetRelativeScale3D(FVector(FieldSize));
}