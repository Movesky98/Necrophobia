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

	// ���� �ִϸ��̼� ������ ���� �Լ��� ����
	void PlayAttackMontage();
	void PlayAppearMontage();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AppearMontage;

private:
	// �̵��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
};
