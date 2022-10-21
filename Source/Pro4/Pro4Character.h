// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Projectile.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.generated.h"

USTRUCT()
struct FGrenadeInfo
{
	GENERATED_BODY()
	UStaticMesh* SM_Grenade = nullptr;
	uint16 GrenadeNum = 0;
	UStaticMesh* SM_Smoke = nullptr;
	uint16 SmokeNum = 0;
	UStaticMesh* SM_Flash = nullptr;
	uint16 FlashNum = 0;
};

USTRUCT()
struct FArmorInfo
{
	GENERATED_BODY()
	bool bHaveArmor = false;
	float AP = 50.0f;
	FString ArmorName = "";
	USkeletalMesh* ArmorMesh;
};

USTRUCT()
struct FWeaponInfo
{
	GENERATED_BODY()
	bool bHaveWeapon = false;
	FString Name = "";
	FString IconPath = "";
	FString ImagePath = "";
	USkeletalMesh* Weapon = nullptr;
};

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

	void SetPlayerWeapon(class AAWeapon* SetWeapon);
	void SetPlayerArmor(class AAArmor* Armor);
	void AddPlayerGrenade(class AAGrenade* _Grenade);
	void DetectZombieSpawner(bool isNight);
	
	APro4PlayerController* GetPlayerController();
	void SetPlayerController(APro4PlayerController* PlayerController);

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
		
	UPROPERTY(VisibleAnywhere, Category = "Player")
	USkeletalMeshComponent* Helmet;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	USkeletalMeshComponent* Vest;

	UPROPERTY(VisibleAnywhere, Category= "Player")
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	UStaticMeshComponent* Grenade;

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

	void Encroached()
	{
		IsEncroach = true;
	}

	void UnEncroached()
	{
		IsEncroach = false;
	}

#pragma region PlayerState
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxHP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxAP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentAP;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void RecoverPlayerHealthOnServer();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void PlayerHealthGetDamagedOnServer(float Damage);

	void GetDamaged(float Damage);

private:
	UPROPERTY(Replicated)
	bool bIsPlayerGetAttacked = false;

	UPROPERTY(Replicated)
	bool bIsRecoveryTimerStarted = false;

	FTimerHandle HealthRecoveryTimer;
	FTimerDelegate HealthRecoveryDelegate;

	FArmorInfo PlayerHelmet;
	FArmorInfo PlayerVest;

	FWeaponInfo MainWeapon;
	FWeaponInfo SubWeapon;
	FWeaponInfo Knife;

	FGrenadeInfo PlayerGrenade;
#pragma endregion


private:
	void NotifyActorBeginOverlap(AActor* Act) override;
	void NotifyActorEndOverlap(AActor* Act) override;

	// 주요 클래스
	APro4PlayerController* PlayerController;
	
	// 초기 세팅
	void MovementSetting();
	void CameraSetting();
	void WeaponSetting();
	void StateSetting();
	void SocketSetting();

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

	/* 플레이어 UI 교체 함수 */
	void ChangePlayerWidget();

	float CameraRotationX;

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

	
	float EncroachTime;
	int32 EncroachLevel;
	bool IsEncroach;

	FTimerHandle FireDelay;

	

	UFUNCTION()
		void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		bool IsEquipping;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		int32 Equipflag;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		bool IsReloading;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float temp;

	UPROPERTY()
		class UPro4AnimInstance* Pro4Anim;

	// Character Role Test.
	FString GetEnumRole(ENetRole CharacterRole);

	/* Trace Sector */
	void CheckFrontActorUsingTrace();

	/* Spawn Projectile Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnProjectileOnServer(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnGrenadeOnServer(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner);

	/* Spawn Armor Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnArmorItemOnServer(FVector Location, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SpawnArmorItemOnClient(class AAArmor* SpawnArmor, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP);

	UFUNCTION(Server, Reliable)
	void NoticePlayerArmorOnServer(class AAArmor* _Armor, const FString& ArmorType);

	UFUNCTION(NetMulticast, Reliable)
	void NoticePlayerArmorOnClient(class AAArmor* _Armor, const FString& ArmorType);

	/* Spawn Weapon Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnWeaponItemOnServer(FVector Location, USkeletalMesh* WeaponMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath);

	UFUNCTION(NetMulticast, Reliable)
	void SpawnWeaponItemOnClient(class AAWeapon* SpawnWeapon, USkeletalMesh* WeaponMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath);

	UFUNCTION(Server, Reliable)
	void NoticePlayerWeaponOnServer(AAWeapon* _Weapon);

	UFUNCTION(NetMulticast, Reliable)
	void NoticePlayerWeaponOnClient(AAWeapon* _Weapon);

	/* 아이템 획득 시, 해당 아이템을 공통적으로 제거하는 함수 */
	UFUNCTION(Server, Reliable)
	void Server_DestroyItem(AActor* DestroyActor);

	/* Detect Zombie Spawner Sector */
	UPROPERTY(VisibleAnywhere, Category = DetectZSpawner)
	UBoxComponent* DetectZSpawnerCol;
	bool IsDayChanged = false;
	FVector DetectExtent = FVector(1000.0f, 1000.0f, 1000.0f);

	uint16 SpawnZombieCurCount = 0;
	uint16 SpawnZombieMaxCount = 20;

	UFUNCTION()
	void ZombieSpawnerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ZombieSpawnerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};