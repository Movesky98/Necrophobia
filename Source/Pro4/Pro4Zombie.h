// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Zombie.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWakeUpEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackStarttedDelegate);

UCLASS()
class PRO4_API APro4Zombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APro4Zombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region ZombieState
	float CurrentHP;
	float Damage;
	float Velocity;

#pragma endregion
	bool IsFind;
	bool IsAttacking;
	bool IsDowning;
	bool IsRun = false;
	bool IsDown;
	bool IsMontagePlay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float CountWakeUp;

	int32 AttackNum;

	UPROPERTY()
		class UZombieAnimInstance* ZombieAnim;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnWakeUpMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/* Zombie Spanwer Collision */
	UPROPERTY(VisibleAnywhere, Category = "ZombieCollision")
	UCapsuleComponent* ZombieCollision;

	UFUNCTION()
	void ZombieEndOverlapToSpawner(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void MovementSetting();

	/* Zombie Get Damaged */
	void ZombieGetDamaged(float _Damage);

	UFUNCTION(Server, Reliable)
	void ZombieGetDamagedOnServer(float _Damage);

	float GetCurrentHP()
	{
		return CurrentHP;
	}

	void SetCurrentHP(float _CurrentHP)
	{
		CurrentHP = _CurrentHP;
	}

	float GetZombieDamage()
	{
		return Damage;
	}

	void SetZombieDamage(float _Damage)
	{
		Damage = _Damage;
	}

	float GetZombieVelocity()
	{
		return Velocity;
	}
	
	void SetZombieVelocity(float _Velocity)
	{
		Velocity = _Velocity;
	}

	void ZombieRun()
	{
		IsRun = true;
	
	}
	void ZombieRunf()
	{
		IsRun = false;
	}

	bool ZombieRunning()
	{
		return IsRun;
	}

	bool ZombieDowning()
	{
		return IsDowning;
	}

	bool DownAnimCheck()
	{
		return IsDown;
	}

	void ZombieAnimStand()
	{
		IsDown = false;
	}

	void FindingT()
	{
		IsFind = true;
	}

	void FindingF()
	{
		IsFind = false;
	}

	void Attack();
	void WakeUp();

	FOnAttackEndDelegate OnAttackEnd;
	FOnWakeUpEndDelegate OnWakeUpEnd;
	FOnAttackStarttedDelegate OnAttackStart;
};
