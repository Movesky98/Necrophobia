
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4AnimInstance.h"
#include "Pro4Character.h"

#include "Net/UnrealNetwork.h"
/**/
UPro4AnimInstance::UPro4AnimInstance()
{
	// ���� �ʱ� ����
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsCrouch = false;
	IsRun = false;
	Equipflag = 0;

	// ���� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EQUIP_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Equip_Montage.Equip_Montage"));
	if (EQUIP_MONTAGE.Succeeded())
	{
		EquipMontage = EQUIP_MONTAGE.Object;
	}

	// ���� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Reload_Montage.Reload_Montage"));
	if (RELOAD_MONTAGE.Succeeded())
	{
		ReloadMontage = RELOAD_MONTAGE.Object;
	}

	// ��� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Attack_Montage.Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// ��ġ �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PUNCH_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Punch_Montage.Punch_Montage"));
	if (PUNCH_MONTAGE.Succeeded())
	{
		PunchMontage = PUNCH_MONTAGE.Object;
	}

	// �ǰ� �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BEATTACKED_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/beAttacked_Montage.beAttacked_Montage"));
	if (BEATTACKED_MONTAGE.Succeeded())
	{
		beAttackedMontage = BEATTACKED_MONTAGE.Object;
	}

	// ��ô �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> THROW_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Throw_Montage.Throw_Montage"));
	if (THROW_MONTAGE.Succeeded())
	{
		ThrowMontage = THROW_MONTAGE.Object;
	}

	// �帵ũ �ִϸ��̼� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DRINK_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Drink_Montage.Drink_Montage"));
	if (DRINK_MONTAGE.Succeeded())
	{
		DrinkMontage = DRINK_MONTAGE.Object;
	}
}

void UPro4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �ִϸ��̼��� ������ ĳ���� ��������
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		// ĳ������ �̵��ӵ�
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Character>(Pawn);
		// ĳ���� Ŭ������ ������ �޾� �ִϸ��̼��� ���� ����
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsCrouch = Character->GetMovementComponent()->IsCrouching();
			IsRun = Character->IsRunning();
			IsZoom = Character->IsZooming();
			IsThrow = Character->ATWThrow();
			Equipflag = Character->IsEquip();
			Moveflag = Character->MoveMode();
			CharacterRotationPitch = Character->CharacterPitch();
			CharacterArmControl = Character->CharacterArmPitch();
		}
	}
}

// ���� ��Ÿ�� ����
void UPro4AnimInstance::PlayEquipMontage()
{
	Montage_Play(EquipMontage, 1.0f);
}

// ���� ��Ÿ�� ����
void UPro4AnimInstance::PlayReloadMontage()
{
	Montage_Play(ReloadMontage, 1.0f);
}

// ���� ��Ÿ�� ����
void UPro4AnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

// ��ġ ��Ÿ�� ����
void UPro4AnimInstance::PlayPunchMontage()
{
	Montage_Play(PunchMontage, 1.0f);
}

// �ǰ� ��Ÿ�� ����
void UPro4AnimInstance::PlaybeAttackedMontage()
{
	Montage_Play(beAttackedMontage, 1.0f);
}

// ��ô ��Ÿ�� ����
void UPro4AnimInstance::PlayThrowMontage()
{
	Montage_Play(ThrowMontage, 1.0f);
}

// ��Ÿ�� ��ȣ �̵�
void UPro4AnimInstance::JumpToEquipMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetEquipMontageSectionName(NewSection), EquipMontage);
}

FName UPro4AnimInstance::GetEquipMontageSectionName(int32 Section)
{
	UE_LOG(Pro4, Log, TEXT("section2."));
	return FName(*FString::Printf(TEXT("%d")), Section);
}

void UPro4AnimInstance::AnimNotify_ThrowNotify()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Throw Notify"));
	
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		APro4Character* Player = Cast<APro4Character>(Pawn);
		Player->ThrowGrenade();
	}
}

UAnimMontage* UPro4AnimInstance::GetEquipMontage()
{
	return EquipMontage;
}

UAnimMontage* UPro4AnimInstance::GetReloadMontage()
{
	return ReloadMontage;
}

UAnimMontage* UPro4AnimInstance::GetAttackMontage()
{
	return AttackMontage;
}

UAnimMontage* UPro4AnimInstance::GetPunchMontage()
{
	return PunchMontage;
}

UAnimMontage* UPro4AnimInstance::GetbeAttackedMontage()
{
	return beAttackedMontage;
}

UAnimMontage* UPro4AnimInstance::GetThrowMontage()
{
	return ThrowMontage;
}

UAnimMontage* UPro4AnimInstance::GetDrinkMontage()
{
	return DrinkMontage;
}