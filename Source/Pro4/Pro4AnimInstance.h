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

	void PlayEquipMontage();
	void PlayReloadMontage();
	void PlayAttackMontage();
	void JumpToEquipMontageSection(int32 NewSection);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EquipMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsProne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		int32 Equipflag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		int32 Moveflag;

	FName GetEquipMontageSectionName(int32 Section);
};
