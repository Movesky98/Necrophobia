// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "Pro4Zombie.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsRun = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Zombie/NormalMaleZombie/Zombie_Attack_Montage.Zombie_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WAKEUP_MONTAGE(TEXT("/Game/Character_Animation/Zombie/NormalMaleZombie/Zombie_WakeUp_Montage.Zombie_WakeUp_Montage"));
	if (WAKEUP_MONTAGE.Succeeded())
	{
		WakeUpMontage = WAKEUP_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEAD_MONTAGE(TEXT("/Game/Character_Animation/Zombie/NormalMaleZombie/Zombie_Dead_Montage.Zombie_Dead_Montage"));
	if (DEAD_MONTAGE.Succeeded())
	{
		DeadMontage = DEAD_MONTAGE.Object;
	}
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Zombie>(Pawn);
		if (Character)
		{
			IsRun = Character->ZombieRunning();
			IsDown = Character->DownAnimCheck();
			IsDead = Character->ZombieDead();
		}
	}
}

void UZombieAnimInstance::AnimNotify_AttackHitCheck()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		APro4Zombie* Zombie = Cast<APro4Zombie>(Pawn);
		Zombie->DrawAttackField();
	}
}

UAnimMontage* UZombieAnimInstance::GetAttackMontage()
{
	return AttackMontage;
}

UAnimMontage* UZombieAnimInstance::GetWakeUpMontage()
{
	return WakeUpMontage;
}

UAnimMontage* UZombieAnimInstance::GetDeadMontage()
{
	return DeadMontage;
}