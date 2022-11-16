// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "Pro4Boss.h"

UBossAnimInstance::UBossAnimInstance()
{
	// 보스 공격 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Attack_Montage.Creta_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// 보스 등장 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> APPEAR_MONTAGE(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Appear_Montage.Creta_Appear_Montage"));
	if (APPEAR_MONTAGE.Succeeded())
	{
		AppearMontage = APPEAR_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_DeadMontage(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Die_Montage.Creta_Die_Montage"));
	if (AM_DeadMontage.Succeeded())
	{
		DeadMontage = AM_DeadMontage.Object;
	}

	CurrentPawnSpeed = 0.0f;
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 애니메이션을 작동시킬 캐릭터 클래스 연결
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto BossZombie = Cast<APro4Boss>(Pawn);
		if (BossZombie)
		{
			isDead = BossZombie->GetIsDead();
		}
	}
}

UAnimMontage* UBossAnimInstance::GetAttackMontage()
{
	return AttackMontage;
}

UAnimMontage* UBossAnimInstance::GetAppearMontage()
{
	return AppearMontage;
}

UAnimMontage* UBossAnimInstance::GetDeadMontage()
{
	return DeadMontage;
}