// Fill out your copyright notice in the Description page of Project Settings.


#include "EncroachField.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Pro4Character.h"

// Sets default values
AEncroachField::AEncroachField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NetCullDistanceSquared = 9000000202358128640.0f;
	EncroachMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EncroachMesh"));
	EncroachPivotComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EncroachPoint"));

	RootComponent = EncroachMeshComponent;
	EncroachPivotComponent->SetupAttachment(EncroachMeshComponent);

	EncroachMeshComponent->SetIsReplicated(true);
	EncroachMeshComponent->SetCollisionProfileName(TEXT("EncorachField"));

	EncroachPivotComponent->InitSphereRadius(500.0f);
	EncroachPivotComponent->SetCollisionProfileName(TEXT("EncroachField"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_Encroach(TEXT("/Game/Environment/Props/Display_Ground_TEST/PolluteTile"));
	if (SK_Encroach.Succeeded())
	{
		EncroachMeshComponent->SetStaticMesh(SK_Encroach.Object);
	}

	EncroachMeshComponent->SetRelativeScale3D(FVector(10.0f, 10.0f, 1.0f));

	bReplicates = true;
	bNetLoadOnClient = true;

	Tags.Add("Encroach");
}

// Called when the game starts or when spawned
void AEncroachField::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float SearchRadius = 50.0f;
	// DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
}

void AEncroachField::NotifyActorBeginOverlap(AActor* Act)
{
	UE_LOG(Pro4, Log, TEXT("Its in the encroaching field"));
	
	if (Act->ActorHasTag("Player"))
	{
		APro4Character* PlayerCharacter = Cast<APro4Character>(Act);
		PlayerCharacter->Encroached();
		PlayerCharacter->StartEncroachTimer();
	}
}

void AEncroachField::NotifyActorEndOverlap(AActor* Act)
{
	UE_LOG(Pro4, Log, TEXT("Its out of the encroaching field"));
	if (Act->ActorHasTag("Player"))
	{
		APro4Character* PlayerCharacter = Cast<APro4Character>(Act);
		PlayerCharacter->UnEncroached();
		PlayerCharacter->StopEncroachTimer();
	}
}

// Called every frame
void AEncroachField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

