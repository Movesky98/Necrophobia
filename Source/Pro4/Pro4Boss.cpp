// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Boss.h"
#include "Pro4BossAI.h"
#include "BossAnimInstance.h"

// Sets default values
APro4Boss::APro4Boss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Boss(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Tpose.Creta_Tpose"));
	if (SK_Boss.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Boss.Object);
	}

	AIControllerClass = APro4BossAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_BossAnim(TEXT("/Game/Character_Animation/Zombie/Creta/CretaAnimBlueprint.CretaAnimBlueprint_C"));
	if (SK_BossAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SK_BossAnim.Class);
	}

	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	MovementSetting();
}

// Called when the game starts or when spawned
void APro4Boss::BeginPlay()
{
	Super::BeginPlay();

}

void APro4Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BossAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (BossAnim != nullptr)
	{
		BossAnim->OnMontageEnded.AddDynamic(this, &APro4Boss::OnAttackMontageEnded);
	}
	else
	{
		UE_LOG(Pro4, Warning, TEXT("ZombieAnimInstance is empty."));
	}
}

void APro4Boss::MovementSetting()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 90.0f);
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

// Called every frame
void APro4Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APro4Boss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APro4Boss::Attack()
{
	if (IsAttacking) return;
	AttackNum = FMath::RandRange(1, 2);
	BossAnim->PlayAttackMontage();

	switch (AttackNum)
	{
	case 1:
		BossAnim->Montage_JumpToSection(FName("1"), BossAnim->AttackMontage);
		break;
	case 2:
		BossAnim->Montage_JumpToSection(FName("2"), BossAnim->AttackMontage);
		break;
	default:
		break;
	}
	IsAttacking = true;
	IsMontagePlay = true;
}

void APro4Boss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontagePlay = false;
	OnAttackEnd.Broadcast();
}
