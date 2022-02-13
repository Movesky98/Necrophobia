// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Projectile.h"

// Sets default values
APro4Projectile::APro4Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;
	Mesh->SetupAttachment(CollisionComponent);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	//ProjectileMovementComponent->Bounciness = 0.3f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_PROJECTILE(TEXT("/Game/Tree/Tree_1/Tree_1-1.Tree_1-1"));
	if (SK_PROJECTILE.Succeeded())
	{
		Mesh->SetStaticMesh(SK_PROJECTILE.Object);
	}

	InitialLifeSpan = 3.0f;
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