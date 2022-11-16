// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Boss.h"
#include "Pro4BossAI.h"
#include "BossAnimInstance.h"
#include "Pro4Character.h"
#include "InGamePlayerState.h"

#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APro4Boss::APro4Boss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 보스 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Boss(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Tpose.Creta_Tpose"));
	if (SK_Boss.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Boss.Object);
	}

	// 보스 캐릭터를 조종할 AI 컨트롤러
	AIControllerClass = APro4BossAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("Zombie"));

	// 보스 애니메이션 연결
	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_BossAnim(TEXT("/Game/Character_Animation/Zombie/Creta/CretaAnimBlueprint.CretaAnimBlueprint_C"));
	if (SK_BossAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SK_BossAnim.Class);
	}

	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	MovementSetting();

	IsAppear = false;

	CurrentHP = 0;
	Damage = 40.0f;
}

// Called when the game starts or when spawned
void APro4Boss::BeginPlay()
{
	Super::BeginPlay();

}

// 몽타주 중복실행을 막기위한 함수
void APro4Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BossAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (BossAnim != nullptr)
	{
		BossAnim->OnMontageEnded.AddDynamic(this, &APro4Boss::OnAttackMontageEnded);
		BossAnim->OnMontageEnded.AddDynamic(this, &APro4Boss::OnAppearMontageEnded);
	}
	else
	{
		UE_LOG(Pro4, Warning, TEXT("ZombieAnimInstance is empty."));
	}
}

// 움직임 초기 설정
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

// 공격
void APro4Boss::Attack()
{
	if (IsAttacking) return;

	// 2가지 공격모션중 랜덤하게 실행
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

void APro4Boss::Appear()
{
	BossAnim->PlayAppearMontage();
	IsMontagePlay = true;
}

// 공격 몽타주 종료시 실행
void APro4Boss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontagePlay = false;
	OnAttackEnd.Broadcast();
}

// 등장 몽타주 종료시 실행
void APro4Boss::OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAppear = true;
	IsMontagePlay = false;
}

void APro4Boss::DrawAttackField()
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
			APro4Character* PlayerCharacter = Cast<APro4Character>(AttackHit.GetActor());
			PlayerCharacter->GetDamaged(Damage, this);
		}
	}
}

void APro4Boss::ZombieGetDamaged(float _Damage, AActor* AttackActor)
{
	if (GetWorld()->IsServer())
	{
		ZombieGetDamagedOnServer(_Damage, AttackActor);
	}
}

void APro4Boss::ZombieGetDamagedOnServer_Implementation(float _Damage, AActor* AttackActor)
{
	CurrentHP -= _Damage;

	if (CurrentHP <= 0.0f)
	{
		if (IsDead) return;
		if (IsMontagePlay)
			BossAnim->Montage_Stop(0.0f);

		// PlayMontageOnServer(BossAnim->GetDeadMontage());
		IsDead = true;
		IsMontagePlay = true;

		// 좀비 킬수를 플레이어에게 저장하도록 구현 필요.
		if (AttackActor->ActorHasTag("Player"))
		{
			APro4Character* AttackPlayer = Cast<APro4Character>(AttackActor);

			AInGamePlayerState* AttackPlayerState = Cast<AInGamePlayerState>(AttackPlayer->GetPlayerState());

			FString TargetType = "Zombie";
			AttackPlayerState->UpdatePlayerKillInfo(TargetType, AttackActor);
		}


		Dead();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Zombie is dead."));
	}
}

void APro4Boss::Dead()
{
	if (GetWorld()->IsServer())
	{
		SetLifeSpan(3.0f);
	}
}

void APro4Boss::PlayMontageOnServer_Implementation(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0)
{
	PlayMontageOnClient(AnimationMontage, SectionNumber);
}

void APro4Boss::PlayMontageOnClient_Implementation(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0)
{
	if (!BossAnim->Montage_IsPlaying(AnimationMontage))
	{
		BossAnim->Montage_Play(AnimationMontage, 1.0f);
		if (SectionNumber)
		{
			FName Section(FString::FromInt(SectionNumber));
			BossAnim->Montage_JumpToSection(Section, AnimationMontage);
		}
	}
}

void APro4Boss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APro4Boss, IsDead);
	DOREPLIFETIME(APro4Boss, CurrentHP);
}