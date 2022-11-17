// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Character.h"
#include "Pro4Projectile.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Pro4PlayerController.h"
#include "Pro4Character.generated.h"

USTRUCT()
struct FGrenadeInfo // ��ô���� ����
{
	GENERATED_BODY()
	UStaticMesh* SM_Grenade = nullptr;	// ����ź �޽�
	uint16 GrenadeNum = 0;				// ����ź ������
	UStaticMesh* SM_Smoke = nullptr;	// ����ź �޽�
	uint16 SmokeNum = 0;				// ����ź ������
	UStaticMesh* SM_Flash = nullptr;	// ����ź �޽�
	uint16 FlashNum = 0;				// ����ź ������
	FString EquipGrenade = "";			// ���� ����ִ� ��ô���� 
};

USTRUCT()
struct FArmorInfo // �� ����
{
	GENERATED_BODY()
	bool bHaveArmor = false; // �� ���� ����
	float AP = 0.0f; // ����
	FString ArmorName = ""; // �� �̸�(���, ��ź����)
	USkeletalMesh* ArmorMesh; // �� �޽�
};

USTRUCT()
struct FWeaponInfo // ���� ����
{
	GENERATED_BODY()
	bool bHaveWeapon = false; // ���� ���� ����
	FString Name = ""; // ���� ���� SR, AR, Pistol
	FString IconPath = ""; // �κ��丮���� ������ ���� Icon ���
	FString ImagePath = ""; // �κ��丮���� ������ ���� �̹��� ���
	USkeletalMesh* Weapon = nullptr; // ���� �޽�
	UStaticMesh* Scope = nullptr; // ���⿡ ������ ������ �޽�
	uint16 CurrentRound = 0; // ���� ������ �Ѿ� ��
	uint16 TotalRound = 0; // ���� ������ �Ѿ� ��
	uint16 Magazine = 0; // źâ ũ��
};

UENUM()
enum class WeaponMode // ���� ����
{
	Main, // �ֹ��� 1��
	Sub, // ��������
	Knife, // Į
	ATW, // ��ô����
	Disarming // ����
};

UCLASS()
class PRO4_API APro4Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APro4Character(); // ������

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WeaponMode CurrentWeaponMode; // ���� ������ ���� ����

	enum class CharacterState // ĳ���� ���� ����
	{
		Standing,
		Crouching,
	};

	CharacterState CurrentCharacterState; // ���� ĳ���� ����

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // ����� �Է��� �޴� �Լ�

	// ĳ���� ���������� ����(����, ��)
	void SetPlayerWeapon(class AAWeapon* SetWeapon);
	void SetPlayerArmor(class AAArmor* Armor);
	void AddPlayerGrenade(class AAGrenade* _Grenade);
	void SetPlayerRound(class AAmmo* _Ammo);
	void EquipGrenade();

	UFUNCTION(Client, Reliable)
	void FlashBangExplosion();

	/* ���� �����ʸ� Ž���ϴ� �ݸ����� Ȱ��ȭ�ϱ� ���� �Լ� */
	void DetectZombieSpawner(bool isNight); 
	
	UFUNCTION(BlueprintCallable)
	void DrawPunch();

	// ��� �ý��� �Լ�
	void StartEncroachTimer();
	UFUNCTION(Server, Reliable)
	void SetPlayerEncroach();
	void StopEncroachTimer();
	UFUNCTION(Server, Reliable)
	void RecoveryEncroach();

	void PlayerEscape(); //
	
	APro4PlayerController* GetPlayerController();
	void SetPlayerController(APro4PlayerController* PlayerController);

	/* ī�޶� ���� */
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, Category = MapCam)
	USpringArmComponent* MapSpringArm;

	UPROPERTY(VisibleAnywhere, Category = MapCam)
	USceneCaptureComponent2D* MapCapture;

	UPROPERTY(VisibleAnywhere, Category = MapCam)
	UPaperSpriteComponent* PaperSprite;
	/* ī�޶� ���� */

	/* �Ѿ� ���� */
	UPROPERTY(EditAnywhere, Category = "GamePlay")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<APro4Projectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	UParticleSystemComponent* MuzzleFlash;
	/* �Ѿ� ���� */
		
	/* ����, �� */
	UPROPERTY(VisibleAnywhere, Category = "Player")
	USkeletalMeshComponent* Helmet;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	USkeletalMeshComponent* Vest;

	UPROPERTY(VisibleAnywhere, Category= "Player")
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	UStaticMeshComponent* Scope;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	UStaticMeshComponent* Grenade;

	UPROPERTY(VisibleAnywhere, Category = "View")
		FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, Category = "View")
		FVector CharacLocation;

	UPROPERTY(VisibleAnywhere, Category = "View")
		FVector CheckView;
	/* ����, �� */

	
	// Ż�� ���� �޼� Ȯ��
	bool GetIsPossibleEscape()
	{
		return IsPossibleEscape;
	}

	UFUNCTION(Server, Reliable)
	void SetIsPossibleEscapeOnServer(bool Escape);

	/* �ٸ� Ŭ�������� ĳ���� ���� Ȯ�ΰ� ������ ���� �Լ��� */
	void Drink();

	bool GetIsDead()
	{
		if (IsDead)
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

	bool ATWThrow()
	{
		return IsThrow;
	}

	bool IsPunching()
	{
		return IsPunch;
	}

	bool IsStabbing()
	{
		return IsStab;
	}

	int32 IsEquip()
	{
		return Equipflag;
	}
	/* �ٸ� Ŭ�������� ĳ���� ���� Ȯ�ΰ� ������ ���� �Լ��� */

	/* ĳ���Ϳ� ī�޶� ȸ������ �ִϸ��̼ǿ� �ݿ��ϱ� ���� �Լ�*/
	float CharacterPitch()
	{
		return CharacterRotationPitch;
	}

	float CharacterArmPitch()
	{
		return CharacterArmControl;
	}
	/* ĳ���Ϳ� ī�޶� ȸ������ �ִϸ��̼ǿ� �ݿ��ϱ� ���� �Լ�*/

	/* ZombieSpawner Sector */
	uint16 GetSpawnZombieCurCount()
	{
		return SpawnZombieCurCount;
	}

	void SetSpawnZombieMaxCount(uint16 _Count)
	{
		SpawnZombieMaxCount = _Count;
	}

	uint16 GetSpawnZombieMaxCount()
	{
		return SpawnZombieMaxCount;
	}

	void SetSpawnZombieCurCount(uint16 _Count)
	{
		SpawnZombieCurCount = _Count;
	}
	/* ZombieSpawner Sector */

	/* �����̴� ������ �ִϸ��̼ǿ� �ݿ��ϱ����� �Լ�*/
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
	/* �����̴� ������ �ִϸ��̼ǿ� �ݿ��ϱ����� �Լ�*/

	/* ��� ���θ� Ȯ���ϱ� ���� �Լ� */
	void Encroached()
	{
		IsEncroach = true;
	}

	void UnEncroached()
	{
		IsEncroach = false;
	}
	/* ��� ���θ� Ȯ���ϱ� ���� �Լ� */

	/* Helicopter */
	UFUNCTION(Server, Reliable)
	void CallHelicopterToEscapeOnServer();

	/* ������ ȹ�� ��, �ش� �������� ���������� �����ϴ� �Լ� */
	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* DestroyActor);

	/* ����ź�� ������ Ÿ�ֿ̹� ����Ǵ� �Լ� */
	UFUNCTION()
	void ThrowGrenade();

#pragma region PlayerState
public:
	/* ĳ���� ü��, ���� */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxHP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxAP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentAP;
	/* ĳ���� ü��, ���� */

	/* �������� ĳ���� ü�� ȸ�� ������ �����ϱ� ���� �Լ� */
	UFUNCTION(Server, Reliable, WithValidation)
	void RecoverPlayerHealthOnServer();

	/* �������� ĳ���� �ǰ� ������ �����ϱ� ���� �Լ� */
	UFUNCTION(Server, Reliable, WithValidation)
	void PlayerHealthGetDamagedOnServer(float Damage, AActor* AttackActor);

	// ĳ���� �ǰ�
	void GetDamaged(float Damage, AActor* AttackActor);

	/* �÷��̾��� ų ������ �����ϴ� �Լ� */
	UFUNCTION(Server, Reliable)
	void RecordPlayerKill(AActor* AttackActor);

	UFUNCTION(Client, Reliable)
	void GameOver();

	/* ���� ȿ���� */
	class UAudioComponent* FireA;
	class USoundCue* SRSound;
	class USoundCue* SubS;
	class USoundCue* EmptyS;
	class USoundCue* FireS;
	/* ���� ȿ���� */

private:
	// ĳ���� �ǰ� ����
	UPROPERTY(Replicated)
	bool bIsPlayerGetAttacked = false;

	/* ĳ���� ü�� ȸ�� ����*/
	UPROPERTY(Replicated)
	bool bIsRecoveryTimerStarted = false;

	FTimerHandle HealthRecoveryTimer;
	FTimerDelegate HealthRecoveryDelegate;
	/* ĳ���� ü�� ȸ�� ����*/

	/* ĳ���� �� ������ ���� ����ü ���� */
	FArmorInfo PlayerHelmet;
	FArmorInfo PlayerVest;
	/* ĳ���� �� ������ ���� ����ü ���� */

	/* ĳ���� ���� ������ ���� ����ü ���� */
	FWeaponInfo MainWeapon;
	FWeaponInfo SubWeapon;
	FWeaponInfo Knife;

	FGrenadeInfo PlayerGrenade;
	/* ĳ���� ���� ������ ���� ����ü ���� */

	class UNecrophobiaGameInstance* NecGameInstance;
#pragma endregion

private:
	void NotifyActorBeginOverlap(AActor* Act) override;
	void NotifyActorEndOverlap(AActor* Act) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHeli_AH64D> BP_Helicopter;

	// �ֿ� Ŭ����
	APro4PlayerController* PlayerController;
	
	// �ʱ� ����
	void MovementSetting();
	void CameraSetting();
	void WeaponSetting();
	void StateSetting();
	void SocketSetting();

	//  ������ ���� �Լ�
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Run();
	void beCrouch();
	void Jump();
	float UpdownSpeed();
	float LeftRightSpeed();
	void ViewPoint();

	// ���� ���� �Լ�
	void Zoom();
	void Attack();
	void StartFire();
	void StopFire();
	void Fire();
	void Fire_Mod();
	void Throw();
	void Punch();
	void Stab();

	// ���� �Լ�
	void EquipMain();
	void EquipSub();
	void EquipKnife();
	void EquipATW();
	void Reload();
	void InteractPressed();

	/* �÷��̾� UI ��ü �Լ� */
	void ChangePlayerWidget();

	float CameraRotationX;

	// �����÷���
	UPROPERTY(Replicated)
	bool IsRun;
	bool IsHold;
	bool EquipAnim;
	bool FireMod;
	UPROPERTY(Replicated)
	bool IsZoom;
	UPROPERTY(Replicated)
	bool IsDead;
	bool bHit;
	bool IsForward;
	bool IsFire;
	bool IsPunch;
	bool IsStab;
	bool IsMontagePlay;
	bool IsPossibleEscape;
	bool CanZoom;
	bool PlayerRun;
	bool IsThrow;
	UPROPERTY(Replicated)
	bool IsDrink;

	int32 Updownflag;
	int32 LeftRightflag;

	// �ִϸ��̼� ��Ʈ���� ���� �÷���
	float HoldTime;
	float MoveRate;
	int32 HoldFlag;
	UPROPERTY(Replicated)
	int32 Moveflag;

	// ��� ���� ����
	float EncroachTime;
	FTimerHandle EncroachTimer;
	int32 EncroachLevel = 0;
	bool IsEncroach;

	// ĳ���� �ִϸ��̼� ��Ʈ�ѿ� ����� ȸ���� ���� ����
	float CharacterRotationPitch;
	float CharacterArmControl;

	// ����ӵ� ���� ����
	FTimerHandle FireDelay;

	

	/* �ִϸ��̼� ��Ÿ�� ����� �ݹ�Ǵ� �Լ� */
	UFUNCTION()
		void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnbeAttackedMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnThrowMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnDrinkMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnPunchMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnStabMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	/* �ִϸ��̼� ��Ÿ�� ����� �ݹ�Ǵ� �Լ� */

	/* �ִϸ��̼� ��Ÿ�� �۵������� üũ�ϱ� ���� ���� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		bool IsEquipping;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
		int32 Equipflag;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
		bool IsReloading;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		bool IsbeAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		bool IsThrowing;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = beAttacked, Meta = (AllowPrivateAccess = true))
		bool IsDrinking;
	/* �ִϸ��̼� ��Ÿ�� �۵������� üũ�ϱ� ���� ���� */

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float temp;

	// �ִϸ��̼� ������ ���� Ŭ���� ����
	UPROPERTY()
		class UPro4AnimInstance* Pro4Anim;

	// Character Role Test.
	FString GetEnumRole(ENetRole CharacterRole);

	/* Spawn Projectile & Grenade Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnProjectileOnServer(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnGrenadeOnServer(const FString& GrenadeType, FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner);

	void SubtractGrenade();
	/* Spawn Projectile & Grenade Section */

	/* Spawn Armor Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnArmorItemOnServer(FVector Location, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SpawnArmorItemOnClient(class AAArmor* SpawnArmor, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP);

	UFUNCTION(Server, Reliable)
	void NoticePlayerArmorOnServer(class AAArmor* _Armor, const FString& ArmorType);

	UFUNCTION(NetMulticast, Reliable)
	void NoticePlayerArmorOnClient(class AAArmor* _Armor, const FString& ArmorType);
	/* Spawn Armor Section */

	/* Spawn Weapon Section */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnWeaponItemOnServer(FVector Location, USkeletalMesh* WeaponMesh, UStaticMesh* ScopeMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath);

	UFUNCTION(Server, Reliable)
	void NoticePlayerWeaponOnServer(AAWeapon* _Weapon);

	UFUNCTION(NetMulticast, Reliable)
	void NoticePlayerWeaponOnClient(AAWeapon* _Weapon);
	/* Spawn Weapon Section */

	/* Detect Zombie Spawner Sector */
	UPROPERTY(VisibleAnywhere, Category = DetectZSpawner)
	UBoxComponent* DetectZSpawnerCol;
	bool IsDayChanged = false;
	FVector DetectExtent = FVector(1000.0f, 1000.0f, 1000.0f);

	// ���� ������ ������� �ִ� ���� ��
	UPROPERTY(Replicated)
	uint16 SpawnZombieCurCount = 0;
	uint16 SpawnZombieMaxCount = 20;

	/* ���� ������ */
	UFUNCTION()
	void ZombieSpawnerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ZombieSpawnerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/* ���� ������ */

	/* Equip Player Weapon Sector */
	UFUNCTION(Server, Reliable)
	void EquipPlayerWeaponOnServer(const WeaponMode& _CurWeaponMode, UStaticMesh* GrenadeMesh = nullptr);

	UFUNCTION(NetMulticast, Reliable)
	void EquipPlayerWeaponOnClient(const WeaponMode& _CurWeaponMode, UStaticMesh* GrenadeMesh = nullptr);
	/* Equip Player Weapon Sector */

	UFUNCTION(Server, Reliable)
	void PlayMontageOnServer(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontageOnClient(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0);

	UFUNCTION(Server, Reliable)
	void SetPlayerStateOnServer(const FString& State, bool bIsState);
	
	UFUNCTION(Server, Reliable)
	void SetPlayerFlagOnServer(const FString& State, int32 Flag);

	UFUNCTION(Client, Reliable)
	void PlayerDead();

	/* �÷��̾ Ż�� ���ǿ� �������� ��  */
	UFUNCTION(Server, Reliable)
	void SuccessPlayerEscapeOnServer();

	UFUNCTION(Client, Reliable)
	void SuccessPlayerEscapeOnClient();

	/* �÷��̾� �̴ϸ� ���� */
	UFUNCTION(Server, Reliable)
	void SetTextureTargetOnServer();

	UFUNCTION(Client, Reliable)
	void SetTextureTargetOnClient();

	UPaperSprite* RenderIcon;

	UTextureRenderTarget2D* RenderTarget;
};