
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4AnimInstance.h"
#include "Pro4Character.h"

#include "Net/UnrealNetwork.h"
/**/
UPro4AnimInstance::UPro4AnimInstance()
{
	// 변수 초기 설정
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsCrouch = false;
	IsRun = false;
	Equipflag = 0;

	// 장착 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EQUIP_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Equip_Montage.Equip_Montage"));
	if (EQUIP_MONTAGE.Succeeded())
	{
		EquipMontage = EQUIP_MONTAGE.Object;
	}

	// 장전 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Reload_Montage.Reload_Montage"));
	if (RELOAD_MONTAGE.Succeeded())
	{
		ReloadMontage = RELOAD_MONTAGE.Object;
	}

	// 사격 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Attack_Montage.Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// 펀치 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PUNCH_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Punch_Montage.Punch_Montage"));
	if (PUNCH_MONTAGE.Succeeded())
	{
		PunchMontage = PUNCH_MONTAGE.Object;
	}

	// 피격 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BEATTACKED_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/beAttacked_Montage.beAttacked_Montage"));
	if (BEATTACKED_MONTAGE.Succeeded())
	{
		beAttackedMontage = BEATTACKED_MONTAGE.Object;
	}

	// 투척 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> THROW_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Throw_Montage.Throw_Montage"));
	if (THROW_MONTAGE.Succeeded())
	{
		ThrowMontage = THROW_MONTAGE.Object;
	}

	// 드링크 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DRINK_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Drink_Montage.Drink_Montage"));
	if (DRINK_MONTAGE.Succeeded())
	{
		DrinkMontage = DRINK_MONTAGE.Object;
	}
}

void UPro4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 애니메이션을 적용할 캐릭터 가져오기
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		// 캐릭터의 이동속도
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Character>(Pawn);
		// 캐릭터 클래스의 변수를 받아 애니메이션의 변수 설정
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

// 장착 몽타주 실행
void UPro4AnimInstance::PlayEquipMontage()
{
	Montage_Play(EquipMontage, 1.0f);
}

// 장전 몽타주 실행
void UPro4AnimInstance::PlayReloadMontage()
{
	Montage_Play(ReloadMontage, 1.0f);
}

// 공격 몽타주 실행
void UPro4AnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

// 펀치 몽타주 실행
void UPro4AnimInstance::PlayPunchMontage()
{
	Montage_Play(PunchMontage, 1.0f);
}

// 피격 몽타주 실행
void UPro4AnimInstance::PlaybeAttackedMontage()
{
	Montage_Play(beAttackedMontage, 1.0f);
}

// 투척 몽타주 실행
void UPro4AnimInstance::PlayThrowMontage()
{
	Montage_Play(ThrowMontage, 1.0f);
}

// 몽타주 번호 이동
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