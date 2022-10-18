// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Projectile.h"
#include "Pro4Character.h"

// Sets default values
APro4Projectile::APro4Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	RootComponent = CollisionComponent;
	Mesh->SetupAttachment(CollisionComponent);

	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetUseCCD(true);
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	//ProjectileMovementComponent->Bounciness = 0.3f;
	// Bullet : /Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_PROJECTILE(TEXT("/Game/MOUT_Fabrication/Meshes/Barracks/SM_FuelWaterSystem/SM_Fuel_Water_System"));
	if (SK_PROJECTILE.Succeeded())
	{
		Mesh->SetStaticMesh(SK_PROJECTILE.Object);
	}

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void APro4Projectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APro4Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APro4Projectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void APro4Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, OtherActor->GetName());
		APro4Character* PlayerCharacter = Cast<APro4Character>(OtherActor);
		PlayerCharacter->GetDamaged(30.0f);
	}
}
