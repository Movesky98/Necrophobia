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

	/* Boss Attack */
	UFUNCTION(BlueprintCallable)
	void StartSwordAttackField();

	UFUNCTION(BlueprintCallable)
	void DrawAttackField();

	UFUNCTION(BlueprintCallable)
	void StopSwordAttackField();

	UFUNCTION(BlueprintCallable)
	void ActivateLeftHandCol();

	UFUNCTION(BlueprintCallable)
	void DeactivateLeftHandCol();


	/* Boss Get Damaged */
	void ZombieGetDamaged(float _Damage, AActor* AttackActor);

	void MovementSetting();
	virtual void PostInitializeComponents() override;
	void Attack();
	void Appear();

	bool CheckAppear()
	{
		return IsAppear;
	}

	bool GetIsDead()
	{
		return IsDead;
	}

	// �����忡 ������ �������� �ݹ����ֱ� ���� ����
	FOnAttackEndDelegate OnAttackEnd;

private:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float Damage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float Velocity;

	// �ִϸ��̼� ��Ʈ���� ���� ����
	UPROPERTY(Replicated)
	bool IsDead;
	UPROPERTY(Replicated)
	bool IsAttacking;
	UPROPERTY(Replicated)
	bool IsMontagePlay;
	UPROPERTY(Replicated)
	bool IsAppear;

	int32 AttackNum;

	// �ִϸ��̼� ������ ���� Ŭ���� ����
	UPROPERTY()
	class UBossAnimInstance* BossAnim;

	// ���� ���� Sword�� �̿��� ������ �� ��, ���Ǵ� Ÿ�̸�
	FTimerHandle SwordAttackTimer;
	FTimerHandle LeftHandAttackTimer;

	/* Zombie Get Damaged */
	UFUNCTION(Server, Reliable)
	void ZombieGetDamagedOnServer(float _Damage, AActor* AttackActor);

	UFUNCTION()
	void LeftHandAttack();

	void Dead();



	/* ��Ÿ�ָ� ��ü Ŭ���̾�Ʈ���� �����ϴ� �Լ� */
	UFUNCTION(Server, Reliable)
	void PlayMontageOnServer(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontageOnClient(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	/* Zombie State Syncronization */
	UFUNCTION(Server, Reliable)
	void SetBossStateOnServer(const FString& State, bool bIsState);

	// ��Ÿ�� ����� �ݵǴ� �Լ�
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
