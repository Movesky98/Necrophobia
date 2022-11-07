// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Boss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PRO4_API APro4Boss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APro4Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MovementSetting();
	virtual void PostInitializeComponents() override;
	void Attack();

	// �����忡 ������ �������� �ݹ����ֱ� ���� ����
	FOnAttackEndDelegate OnAttackEnd;

private:
	// �ִϸ��̼� ��Ʈ���� ���� ����
	bool IsAttacking;
	bool IsMontagePlay;
	int32 AttackNum;

	// �ִϸ��̼� ������ ���� Ŭ���� ����
	UPROPERTY()
		class UBossAnimInstance* BossAnim;

	// ���� ��Ÿ�� ����� �ݵǴ� �Լ�
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
