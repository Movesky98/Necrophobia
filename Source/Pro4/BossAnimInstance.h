// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PRO4_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBossAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/* Get Montage Field */
	UAnimMontage* GetAttackMontage();
	UAnimMontage* GetAppearMontage();
	UAnimMontage* GetDeadMontage();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AppearMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeadMontage;


private:
	// 이동을 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool isDead;
};
