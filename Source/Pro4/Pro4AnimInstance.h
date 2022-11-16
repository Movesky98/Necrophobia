// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "Animation/AnimInstance.h"
#include "Pro4AnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API UPro4AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPro4AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UAnimMontage* GetEquipMontage();
	UAnimMontage* GetReloadMontage();
	UAnimMontage* GetAttackMontage();
	UAnimMontage* GetPunchMontage();
	UAnimMontage* GetbeAttackedMontage();
	UAnimMontage* GetThrowMontage();
	UAnimMontage* GetDrinkMontage();

	void PlayEquipMontage();
	void PlayReloadMontage();
	void PlayAttackMontage();
	void PlayPunchMontage();
	void PlaybeAttackedMontage();
	void PlayThrowMontage();

	void JumpToEquipMontageSection(int32 NewSection);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EquipMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PunchMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		UAnimMontage* beAttackedMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ThrowMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DrinkMontage;
private:
	// 좀비 애니메이션 컨트롤을 위한 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsThrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		int32 Equipflag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		int32 Moveflag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CharacterRotationPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CharacterArmControl;

	UFUNCTION()
	void AnimNotify_Punch();

	FName GetEquipMontageSectionName(int32 Section);

	UFUNCTION()
	void AnimNotify_ThrowNotify();
};
