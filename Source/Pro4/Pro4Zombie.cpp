// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Zombie.h"
#include "Pro4ZombieAI.h"
#include "ZombieAnimInstance.h"
#include "ZombieSpawner.h"
#include "Pro4Character.h"

#include "DrawDebugHelpers.h"

/*
* 나중에 좀비 죽었을 때, 플레이어가 소환한 좀비의 수를 줄이도록 구현해야합니다.
*/

// Sets default values
APro4Zombie::APro4Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ZombieCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ZombieCollision"));

	ZombieCollision->SetupAttachment(RootComponent);
	ZombieCollision->SetCollisionProfileName(TEXT("Detect_ZSpawner"));
	ZombieCollision->SetCapsuleHalfHeight(150.0f);
	ZombieCollision->SetCapsuleRadius(150.0f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Zombie(TEXT("/Game/Character_Animation/Zombie/NormalMaleZombie/attack.attack"));
	if (SK_Zombie.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Zombie.Object);
	}

	AIControllerClass = APro4ZombieAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_ZombieAnim(TEXT("/Game/Character_Animation/Zombie/NormalMaleZombie/ZombieAnimBlueprint.ZombieAnimBlueprint_C"));
	if (SK_ZombieAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SK_ZombieAnim.Class);
	}
	
	MovementSetting();
	IsAttacking = false;
	IsDowning = true;
	IsMontagePlay = false;
	IsDown = true;
	IsDead = false;

	CurrentHP = 1.0f;
<<<<<<< HEAD
	Damage = 30.0f;
=======
	Damage = 20.0f;
>>>>>>> 24d5f156ac738fd7e5f71ba5ef89afc460b53dd0
	Velocity = 0.0f;
	Tags.Add("Zombie");
}

// Called when the game starts or when spawned
void APro4Zombie::BeginPlay()
{
	Super::BeginPlay();

	ZombieCollision->OnComponentEndOverlap.AddDynamic(this, &APro4Zombie::ZombieEndOverlapToSpawner);
	
}

void APro4Zombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ZombieAnim = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (ZombieAnim != nullptr) 
	{
		ZombieAnim->OnMontageEnded.AddDynamic(this, &APro4Zombie::OnAttackMontageEnded);
		ZombieAnim->OnMontageEnded.AddDynamic(this, &APro4Zombie::OnWakeUpMontageEnded);
	}
	else
	{
		UE_LOG(Pro4, Warning, TEXT("ZombieAnimInstance is empty."));
	}
}

void APro4Zombie::MovementSetting()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 90.0f);
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

// Called every frame
void APro4Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ZombieRunning())
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}

	if (IsMontagePlay && IsDowning)
	{
		CountWakeUp += DeltaTime;
		if (CountWakeUp > 1.0f)
		{
			IsDown = false;
		}
	}
}

// Called to bind functionality to input
void APro4Zombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APro4Zombie::Attack()
{
	if (IsAttacking) return;
	AttackNum = FMath::RandRange(1, 2);
	ZombieAnim->PlayAttackMontage();

	switch (AttackNum)
	{
	case 1:
		ZombieAnim->Montage_JumpToSection(FName("1"), ZombieAnim->AttackMontage);
		break;
	case 2:
		ZombieAnim->Montage_JumpToSection(FName("2"), ZombieAnim->AttackMontage);
		break;
	default:
		break;
	}

	IsAttacking = true;
	IsMontagePlay = true;
}

void APro4Zombie::WakeUp()
{
	if (!IsDowning) return;
	ZombieAnim->PlayWakeUpMontage();
	IsMontagePlay = true;
}

void APro4Zombie::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontagePlay = false;
	OnAttackEnd.Broadcast();
}

void APro4Zombie::OnWakeUpMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsDowning = false;
}

void APro4Zombie::OnDeadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsDeading = false;
}

void APro4Zombie::ZombieEndOverlapToSpawner(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("ZombieSpawner"))
	{
		AZombieSpawner* ZombieSpawner = Cast<AZombieSpawner>(OtherActor);

		if (ZombieSpawner->GetIsSpawn())
		{
			ZombieSpawner->SetIsSpawn(false);
		}
	}
}

void APro4Zombie::ZombieGetDamaged(float _Damage)
{
	if (GetWorld()->IsServer())
	{
		ZombieGetDamagedOnServer(_Damage);
	}
}

void APro4Zombie::ZombieGetDamagedOnServer_Implementation(float _Damage)
{
	CurrentHP -= _Damage;

	if (CurrentHP <= 0.0f)
	{
		if (IsDead) return;
		if (IsMontagePlay)
			ZombieAnim->Montage_Stop(0.0f);
		ZombieAnim->PlayDeadMontage();
		IsDead = true;
		IsMontagePlay = true;
		IsDeading = true;
<<<<<<< HEAD
=======

		Dead();
>>>>>>> 24d5f156ac738fd7e5f71ba5ef89afc460b53dd0
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Zombie is dead."));
	}
}

<<<<<<< HEAD
void APro4Zombie::Dead()
{

=======

void APro4Zombie::Dead()
{
	if (GetWorld()->IsServer())
	{
		SetLifeSpan(3.0f);
	}
}

void APro4Zombie::DrawAttackField()
{
	FHitResult AttackHit;
	FName Profile = "Zombie";
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 150.0f));
	FVector CollisionLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;

	FCollisionQueryParams GrenadeColParams;
	bool IsHit = GetWorld()->SweepSingleByProfile(AttackHit, CollisionLocation, CollisionLocation, FQuat::Identity, Profile, BoxCollision);
	DrawDebugBox(GetWorld(), CollisionLocation, BoxCollision.GetExtent(), FColor::Red, true, 5.0f, 0, 5.0f);

	if (IsHit)
	{
		if (AttackHit.GetActor()->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Player get damaged."));

			APro4Character* PlayerCharacter = Cast<APro4Character>(AttackHit.GetActor());
			PlayerCharacter->GetDamaged(Damage);
		}
	}
>>>>>>> 24d5f156ac738fd7e5f71ba5ef89afc460b53dd0
}