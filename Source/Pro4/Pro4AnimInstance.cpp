
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4AnimInstance.h"
#include "Pro4Character.h"
/**/
UPro4AnimInstance::UPro4AnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsCrouch = false;
	IsProne = false;
	IsRun = false;
	Equipflag = 0;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EQUIP_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Equip_Montage.Equip_Montage"));
	if (EQUIP_MONTAGE.Succeeded())
	{
		EquipMontage = EQUIP_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Reload_Montage.Reload_Montage"));
	if (RELOAD_MONTAGE.Succeeded())
	{
		ReloadMontage = RELOAD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character_Animation/Mannequin/Animations/Attack_Montage.Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPro4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Character>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsCrouch = Character->GetMovementComponent()->IsCrouching();
			IsProne = Character->IsProning();
			IsRun = Character->IsRunning();
			IsZoom = Character->IsZooming();
			Equipflag = Character->IsEquip();
			Moveflag = Character->MoveMode();
			CharacterRotationPitch = Character->CharacterPitch();
			CharacterRotationYaw = Character->CharacterYaw();
		}
	}
}

void UPro4AnimInstance::PlayEquipMontage()
{
	Montage_Play(EquipMontage, 1.0f);
}

void UPro4AnimInstance::PlayReloadMontage()
{
	Montage_Play(ReloadMontage, 1.0f);
}

void UPro4AnimInstance::PlayAttackMontage()
{
	UE_LOG(Pro4, Log, TEXT("Attack11"));
	Montage_Play(AttackMontage, 1.0f);
}

void UPro4AnimInstance::JumpToEquipMontageSection(int32 NewSection)
{
	UE_LOG(Pro4, Log, TEXT("section1."));
	Montage_JumpToSection(GetEquipMontageSectionName(NewSection), EquipMontage);
}

FName UPro4AnimInstance::GetEquipMontageSectionName(int32 Section)
{
	UE_LOG(Pro4, Log, TEXT("section2."));
	return FName(*FString::Printf(TEXT("%d")), Section);
}