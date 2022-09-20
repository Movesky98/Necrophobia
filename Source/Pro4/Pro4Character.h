// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Projectile.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
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

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, Category = MapCam)
		USpringArmComponent* MapSpringArm;

	UPROPERTY(VisibleAnywhere, Category = MapCam)
		USceneCaptureComponent2D* MapCapture;

	UPROPERTY(EditAnywhere, Category = "GamePlay")
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<APro4Projectile> ProjectileClass;
		

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

	bool IsZooming()
	{
		if (IsZoom)
			return true;
		else
			return false;
	}

	bool EquippingAnim()
	{
		if (EquipAnim)
			return true;
		else
			return false;
	}

	int32 IsEquip()
	{
		return Equipflag;
	}

	int32 MoveMode()
	{
		if (IsForward)
		{
			if (Moveflag == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			if (Moveflag == 0)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}
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
	// 초기 세팅
	void MovementSetting();
	void CameraSetting();
	void WeaponSetting();
	void StateSetting();

	//  움직임 관련 함수
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Run();
	void beCrouch();
	void Prone();
	void Jump();
	float UpdownSpeed();
	float LeftRightSpeed();

	// 공격 관련 함수
	void Zoom();
	void Attack();
	void StartFire();
	void StopFire();
	void Fire();
	void Fire_Mod();
	void Throw();
	void Punch();

	// 장착 함수
	void EquipMain1();
	void EquipMain2();
	void EquipSub();
	void EquipATW();
	void Reload();
	void InteractPressed();

	// 플레이어 UI 교체
	void ChangePlayerWidget();

	// 상태플래그
	bool IsRun;
	bool IsHold;
	bool EquipAnim;
	bool FireMod;
	bool IsZoom;
	bool bHit;
	bool IsForward;
	bool IsFire;
	bool IsMontagePlay;

	int32 Updownflag;
	int32 LeftRightflag;

	float HoldTime;
	float MoveRate; // 이동속도 조절 변수
	int32 HoldFlag;
	int32 Moveflag;

	FTimerHandle FireDelay;

	

	UFUNCTION()
		void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		bool IsEquipping;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		int32 Equipflag;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		bool IsReloading;

	UPROPERTY()
		class UPro4AnimInstance* Pro4Anim;

	// Character Role Test.
	FString GetEnumRole(ENetRole CharacterRole);
};