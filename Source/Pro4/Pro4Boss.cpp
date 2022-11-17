// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Boss.h"
#include "Pro4BossAI.h"
#include "BossAnimInstance.h"
#include "Pro4Character.h"
#include "InGamePlayerState.h"

#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

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

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("Zombie"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(6.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetGenerateOverlapEvents(true);

	// 보스 애니메이션 연결
	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_BossAnim(TEXT("/Game/Character_Animation/Zombie/Creta/CretaAnimBlueprint.CretaAnimBlueprint_C"));
	if (SK_BossAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SK_BossAnim.Class);
	}

	MovementSetting();

	IsAppear = false;

	CurrentHP = 400.0f;
	Damage = 40.0f;
	Tags.Add("BossZombie");
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
	if (!GetWorld()->IsServer()) return;

	// 2가지 공격모션중 랜덤하게 실행
	AttackNum = FMath::RandRange(1, 2);
	PlayMontageOnServer(BossAnim->GetAttackMontage(), AttackNum);

	SetBossStateOnServer("Attack", true);
	SetBossStateOnServer("MontagePlay", true);
}

void APro4Boss::Appear()
{
	PlayMontageOnServer(BossAnim->GetAppearMontage());
	SetBossStateOnServer("MontagePlay", true);
}

// 공격 몽타주 종료시 실행
void APro4Boss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetBossStateOnServer("Attack", false);
	SetBossStateOnServer("MontagePlay", false);
	OnAttackEnd.Broadcast();
}

// 등장 몽타주 종료시 실행
void APro4Boss::OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetBossStateOnServer("Appear", true);
	SetBossStateOnServer("MontagePlay", false);
}

/* 보스 좀비의 공격 필드를 그려내는 함수 */
void APro4Boss::DrawAttackField()
{
	FHitResult AttackHit;
	FName Profile = "Zombie";
	TArray<AActor*> IgnoreActor;
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 50.0f));
	FVector Start = GetMesh()->GetSocketLocation("Sword_Start");
	FVector End = GetMesh()->GetSocketLocation("Sword_End");
	FRotator Rotation = (End - Start).Rotation();

	bool IsHit = UKismetSystemLibrary::BoxTraceSingleByProfile(
		GetWorld(),
		Start,
		End,
		FVector(10.0f),
		Rotation,
		Profile,
		true,
		IgnoreActor,
		EDrawDebugTrace::Persistent,
		AttackHit,
		true);

	if (IsHit)
	{
		if (AttackHit.GetActor()->ActorHasTag("Player"))
		{
			APro4Character* PlayerCharacter = Cast<APro4Character>(AttackHit.GetActor());
			PlayerCharacter->GetDamaged(Damage, this);
			GetWorldTimerManager().ClearTimer(SwordAttackTimer);
		}
	}
}

void APro4Boss::StartSwordAttackField()
{
	GetWorldTimerManager().SetTimer(SwordAttackTimer, this, &APro4Boss::DrawAttackField, 0.01f, true);
}

/* 보스의 공격 필드를 그려내는 것을 그만두는 함수 */
void APro4Boss::StopSwordAttackField()
{
	GetWorldTimerManager().ClearTimer(SwordAttackTimer);
}

/* 보스좀비가 데미지를 입었을 때 실행되는 함수 */
void APro4Boss::ZombieGetDamaged(float _Damage, AActor* AttackActor)
{
	if (GetWorld()->IsServer())
	{
		ZombieGetDamagedOnServer(_Damage, AttackActor);
	}
}

void APro4Boss::LeftHandAttack()
{
	FHitResult AttackHit;
	FName Profile = "Zombie";
	TArray<AActor*> IgnoreActor;
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 50.0f));
	FVector Location = GetMesh()->GetSocketLocation("LeftHand");

	bool IsHit = UKismetSystemLibrary::SphereTraceSingleByProfile(
		GetWorld(),
		Location,
		Location,
		50.0f,
		Profile,
		true,
		IgnoreActor,
		EDrawDebugTrace::Persistent,
		AttackHit,
		true);

	if (IsHit)
	{
		if (AttackHit.GetActor()->ActorHasTag("Player"))
		{
			APro4Character* PlayerCharacter = Cast<APro4Character>(AttackHit.GetActor());
			PlayerCharacter->GetDamaged(Damage, this);
			GetWorldTimerManager().ClearTimer(LeftHandAttackTimer);
		}
	}
}

void APro4Boss::ActivateLeftHandCol()
{
	GetWorldTimerManager().SetTimer(LeftHandAttackTimer, this, &APro4Boss::LeftHandAttack, 0.01f, true);
}

void APro4Boss::DeactivateLeftHandCol()
{
	GetWorldTimerManager().ClearTimer(LeftHandAttackTimer);
}


void APro4Boss::ZombieGetDamagedOnServer_Implementation(float _Damage, AActor* AttackActor)
{
	CurrentHP -= _Damage;

	if (CurrentHP <= 0.0f)
	{
		if (IsDead) return;
		if (IsMontagePlay)
			BossAnim->Montage_Stop(0.0f);

		PlayMontageOnServer(BossAnim->GetDeadMontage());
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

void APro4Boss::SetBossStateOnServer_Implementation(const FString& State, bool bIsState)
{
	if (State == "Attack")
	{
		IsAttacking = bIsState;
	}
	else if (State == "MontagePlay")
	{
		IsMontagePlay = bIsState;
	}
	else if (State == "Dead")
	{
		IsDead = bIsState;
	}
	else if (State == "Appear")
	{
		IsAppear = bIsState;
	}
}

void APro4Boss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APro4Boss, CurrentHP);
	DOREPLIFETIME(APro4Boss, IsDead);
	DOREPLIFETIME(APro4Boss, IsAttacking);
	DOREPLIFETIME(APro4Boss, IsMontagePlay);
	DOREPLIFETIME(APro4Boss, IsAppear);
}