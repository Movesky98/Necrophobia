// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Zombie.h"
#include "Pro4ZombieAI.h"
#include "ZombieAnimInstance.h"

// Sets default values
APro4Zombie::APro4Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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
}

// Called when the game starts or when spawned
void APro4Zombie::BeginPlay()
{
	Super::BeginPlay();
	
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