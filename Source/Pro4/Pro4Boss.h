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

	// 블랙보드에 공격이 끝낫음을 콜백해주기 위한 변수
	FOnAttackEndDelegate OnAttackEnd;

private:
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float Damage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float Velocity;

	// 애니메이션 컨트롤을 위한 변수
	UPROPERTY(Replicated)
	bool IsDead;
	UPROPERTY(Replicated)
	bool IsAttacking;
	UPROPERTY(Replicated)
	bool IsMontagePlay;
	UPROPERTY(Replicated)
	bool IsAppear;

	int32 AttackNum;

	// 애니메이션 적용을 위한 클래스 변수
	UPROPERTY()
	class UBossAnimInstance* BossAnim;

	// 보스 좀비가 Sword를 이용한 공격을 할 때, 사용되는 타이머
	FTimerHandle SwordAttackTimer;
	FTimerHandle LeftHandAttackTimer;

	/* Zombie Get Damaged */
	UFUNCTION(Server, Reliable)
	void ZombieGetDamagedOnServer(float _Damage, AActor* AttackActor);

	UFUNCTION()
	void LeftHandAttack();

	void Dead();



	/* 몽타주를 전체 클라이언트에서 실행하는 함수 */
	UFUNCTION(Server, Reliable)
	void PlayMontageOnServer(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontageOnClient(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	/* Zombie State Syncronization */
	UFUNCTION(Server, Reliable)
	void SetBossStateOnServer(const FString& State, bool bIsState);

	// 몽타주 종료시 콜되는 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
