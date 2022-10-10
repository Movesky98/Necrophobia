// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Zombie.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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
	bool IsFind;

	int32 AttackNum;

	UPROPERTY()
		class UZombieAnimInstance* ZombieAnim;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void MovementSetting();
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		bool IsRun=false;
	
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

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;
};
