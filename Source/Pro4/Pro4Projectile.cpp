// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Projectile.h"
#include "Pro4Character.h"
#include "Pro4Zombie.h"
#include "Pro4Boss.h"

#include "DrawDebugHelpers.h"

// Sets default values
APro4Projectile::APro4Projectile()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE")); 

	RootComponent = Mesh;
	ProjectileParticle->SetupAttachment(Mesh);

	Mesh->SetCollisionProfileName(TEXT("Projectile"));
	Mesh->SetUseCCD(true);

	ProjectileMovementComponent->SetUpdatedComponent(Mesh);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	// Bullet : /Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45
	// Box : /Game/StarterContent/Shapes/Shape_Cube

	ProjectileParticle->bAutoActivate = false;

	/* 필요한 에셋들을 불러옴 */
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_PROJECTILE(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45"));
	if (SK_PROJECTILE.Succeeded())
	{
		Mesh->SetStaticMesh(SK_PROJECTILE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_GroundDirt(TEXT("/Game/Impacts/Particles/Ground_Dirt/P_Ground_Dirt_2"));
	if (P_GroundDirt.Succeeded())
	{
		Particle_GroundDirt = P_GroundDirt.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Ceramic(TEXT("/Game/Impacts/Particles/Ceramic/P_Ceramic_2"));
	if (P_Ceramic.Succeeded())
	{
		Particle_Ceramic = P_Ceramic.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Wood(TEXT("/Game/Impacts/Particles/Wood/Wood_2/P_Wood2_2"));
	if (P_Wood.Succeeded())
	{
		Particle_Wood = P_Wood.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Blood(TEXT("/Game/Impacts/Particles/Blood/P_Blood_2"));
	if (P_Blood.Succeeded())
	{
		Particle_Blood = P_Blood.Object;
	}
}

/* 월드에 생성되었을 때, 실행되는 함수 */
void APro4Projectile::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APro4Projectile::ProjectileBeginOverlap);

}

/* 총알의 발사 방향과 속도를 설정해주는 함수 */
void APro4Projectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

/* 총알이 무언가에 맞았을 때 실행되는 함수 */
void APro4Projectile::ProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProjectileMovementComponent->bSimulationEnabled = false;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FRotator NewRotation = SweepResult.ImpactNormal.Rotation();

	if (OtherActor->ActorHasTag("Player"))
	{
		ProjectileParticle->SetTemplate(Particle_Blood);
		
		APro4Character* PlayerCharacter = Cast<APro4Character>(OtherActor);
		PlayerCharacter->GetDamaged(30.0f, GetOwner());
	}
	else if(OtherActor->ActorHasTag("Zombie"))
	{
		ProjectileParticle->SetTemplate(Particle_Blood);

		APro4Zombie* Zombie = Cast<APro4Zombie>(OtherActor);
		Zombie->ZombieGetDamaged(30.0f, GetOwner());
	}
	else if (OtherActor->ActorHasTag("BossZombie"))
	{
		ProjectileParticle->SetTemplate(Particle_Blood);

		APro4Boss* BossZombie = Cast<APro4Boss>(OtherActor);
		BossZombie->ZombieGetDamaged(30.0f, GetOwner());
	}
	else
	{
		ProjectileParticle->SetTemplate(Particle_Ceramic);
	}

	ProjectileParticle->ToggleActive();
	ProjectileParticle->SetRelativeRotation(NewRotation);

	SetLifeSpan(1.0f);
}
