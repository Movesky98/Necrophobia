// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "Pro4Boss.h"

UBossAnimInstance::UBossAnimInstance()
{
	// ���� ���� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Attack_Montage.Creta_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// ���� ���� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> APPEAR_MONTAGE(TEXT("/Game/Character_Animation/Zombie/Creta/Creta_Appear_Montage.Creta_Appear_Montage"));
	if (APPEAR_MONTAGE.Succeeded())
	{
		AppearMontage = APPEAR_MONTAGE.Object;
	}
	CurrentPawnSpeed = 0.0f;
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// �ִϸ��̼��� �۵���ų ĳ���� Ŭ���� ����
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Boss>(Pawn);
	}
}

// ���� �ִϸ��̼� ����
void UBossAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

// ���� �ִϸ��̼� ����
void UBossAnimInstance::PlayAppearMontage()
{
	if (!Montage_IsPlaying(AppearMontage))
	{
		Montage_Play(AppearMontage, 1.0f);
	}
}