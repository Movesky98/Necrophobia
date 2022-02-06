// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Character.generated.h"

UCLASS()
class PRO4_API APro4Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APro4Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class WeaponMode
	{
		Main1,
		Main2,
		Sub,
		ATW,
		Disarming
	};

	WeaponMode CurrentWeaponMode=WeaponMode::Disarming;

	enum class CharacterState
	{
		Standing,
		Crouching,
		Proning,
	};

	CharacterState CurrentCharacterState = CharacterState::Standing;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	UStaticMeshComponent* Weapon;

	bool IsProning()
	{
		if (CurrentCharacterState == CharacterState::Proning)
			return true;
		else
			return false;
	}

	bool IsRunning()
	{
		if (IsRun)
			return true;
		else
			return false;
	}

#pragma region PlayerState
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxAP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentAP;

#pragma endregion


private:
	void MovementSetting();
	void CameraSetting();
	//void WeaponSetting();
	void StateSetting();
	void Run();
	void Fire();
	void Crouch();
	void Prone();
	void Jump();
	void EquipMain1();
	void EquipMain2();
	void EquipSub();
	void EquipATW();
	void InteractPressed();

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	bool IsRun;
	bool IsHold;
	bool bHit;

	float HoldTime;
	int32 HoldFlag;
};