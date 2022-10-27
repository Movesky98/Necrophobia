// Fill out your copyright notice in the Description page of Project Settings.


#include "Heli_AH64D.h"

#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHeli_AH64D::AHeli_AH64D()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MachineGunFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MachineGunFX"));
	Damaged = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Damaged"));
	DamagedSmoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamagedSmoke"));

	RootComponent = SkeletalMesh;
	MachineGunFX->SetupAttachment(SkeletalMesh);
	Damaged->SetupAttachment(SkeletalMesh);
	DamagedSmoke->SetupAttachment(Damaged);

	Damaged->SetVisibility(false);
	MachineGunFX->bAutoActivate = false;
	DamagedSmoke->bAutoActivate = false;

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
	SetMainRotorSpeed(100.0f);
	SetTailRotorSpeed(100.0f);
}

// Called when the game starts or when spawned
void AHeli_AH64D::BeginPlay()
{
	Super::BeginPlay();
	
	CallEscape();
}

// Called every frame
void AHeli_AH64D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorWorldOffset(GetActorRotation().Vector() * 100.0f);
}

