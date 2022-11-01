// Fill out your copyright notice in the Description page of Project Settings.


#include "Heli_AH64D.h"
#include "Pro4Character.h"

#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHeli_AH64D::AHeli_AH64D()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MachineGunFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MachineGunFX"));
	Damaged = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Damaged"));
	DamagedSmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamagedSmoke"));
	EscapeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EscapeCollision"));

	RootComponent = SkeletalMesh;
	MachineGunFX->SetupAttachment(SkeletalMesh);
	Damaged->SetupAttachment(SkeletalMesh);
	DamagedSmoke->SetupAttachment(Damaged);

	Damaged->SetVisibility(false);
	MachineGunFX->bAutoActivate = false;
	DamagedSmoke->bAutoActivate = false;

	EscapeCollision->SetGenerateOverlapEvents(false);
	EscapeCollision->SetBoxExtent(FVector(100.0f));
	EscapeCollision->SetIsReplicated(true);
	EscapeCollision->SetCollisionProfileName(TEXT("Escape"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Heli(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/SK_West_Heli_AH64D"));
	if (SK_Heli.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SK_Heli.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_MuzzleFlash(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/FX/PS_MuzzleFlash_01_Heli_AH64D"));
	if (PS_MuzzleFlash.Succeeded())
	{
		MachineGunFX->SetTemplate(PS_MuzzleFlash.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Heli(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/Damaged/SM_West_Heli_Ah64D_Damaged"));
	if(SM_Heli.Succeeded())
	{
		Damaged->SetStaticMesh(SM_Heli.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Wreckage(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/FX/PS_Wreckage_01_Heli_AH64D"));
	if (PS_Wreckage.Succeeded())
	{
		DamagedSmoke->SetTemplate(PS_Wreckage.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_ANIM(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/ABP_West_Heli_AH64D.ABP_West_Heli_AH64D_C"));
	if (SK_ANIM.Succeeded())
	{
		SkeletalMesh->SetAnimInstanceClass(SK_ANIM.Class);
	}
}

void AHeli_AH64D::CallEscape()
{
	SetMainRotorSpeed(300.0f);
	SetTailRotorSpeed(300.0f);
}

// Called when the game starts or when spawned
void AHeli_AH64D::BeginPlay()
{
	Super::BeginPlay();
	
	CallEscape();
	EscapeCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeli_AH64D::CheckEscapeCollision);
}

// Called every frame
void AHeli_AH64D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsReachPlayer)
	{
		AddActorWorldOffset(GetActorRotation().Vector() * 30.0f);

		if (FVector::Dist2D(GetActorLocation(), GetTargetPlayerLocation()) <= 100.0f)
		{
			IsReachPlayer = true;
			ActiveEscapeCollision();
		}
	}
}

void AHeli_AH64D::ActiveEscapeCollision()
{
	FHitResult HitResult;

	FVector Start = GetActorLocation();
	FVector End = Start + (-FVector::ZAxisVector * 2000);
	
	FCollisionQueryParams TraceParams;

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (IsHit)
	{
		if (HitResult.Actor != nullptr)
		{
			DrawDebugSolidBox(GetWorld(), HitResult.ImpactPoint, FVector(10.0f), FColor::Green, true);
			EscapeCollision->SetWorldLocation(HitResult.ImpactPoint);
			EscapeCollision->SetGenerateOverlapEvents(true);
		}
	}
}

void AHeli_AH64D::CheckEscapeCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		APro4Character* PlayerChracter = Cast<APro4Character>(OtherActor);
		PlayerChracter->PlayerEscape();
	}
}

void AHeli_AH64D::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeli_AH64D, TargetPlayerLocation);
	DOREPLIFETIME(AHeli_AH64D, IsReachPlayer);
}