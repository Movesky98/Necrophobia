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
struct FGrenadeInfo // 투척무기 정보
{
	GENERATED_BODY()
	UStaticMesh* SM_Grenade = nullptr;	// 수류탄 메시
	uint16 GrenadeNum = 0;				// 수류탄 보유수
	UStaticMesh* SM_Smoke = nullptr;	// 연막탄 메시
	uint16 SmokeNum = 0;				// 연막탄 보유수
	UStaticMesh* SM_Flash = nullptr;	// 섬광탄 메시
	uint16 FlashNum = 0;				// 섬광탄 보유수
	FString EquipGrenade = "";			// 현재 들고있는 투척무기 
};

USTRUCT()
struct FArmorInfo // 방어구 정보
{
	GENERATED_BODY()
	bool bHaveArmor = false; // 방어구 장착 여부
	float AP = 0.0f; // 방어력
	FString ArmorName = ""; // 방어구 이름(헬멧, 방탄조끼)
	USkeletalMesh* ArmorMesh; // 방어구 메시
};

USTRUCT()
struct FWeaponInfo // 무기 정보
{
	GENERATED_BODY()
	bool bHaveWeapon = false; // 무기 보유 여부
	FString Name = ""; // 무기 종류 SR, AR, Pistol
	FString IconPath = ""; // 인벤토리에서 보여질 무기 Icon 경로
	FString ImagePath = ""; // 인벤토리에서 보여질 무기 이미지 경로
	USkeletalMesh* Weapon = nullptr; // 무기 메시
	UStaticMesh* Scope = nullptr; // 무기에 장착될 스코프 메시
	uint16 CurrentRound = 0; // 현재 장전된 총알 수
	uint16 TotalRound = 0; // 현재 보유한 총알 수
	uint16 Magazine = 0; // 탄창 크기
};

UENUM()
enum class WeaponMode // 무기 유형
{
	Main, // 주무기 1번
	Sub, // 보조무기
	Knife, // 칼
	ATW, // 투척무기
	Disarming // 비무장
};

UCLASS()
class PRO4_API APro4Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APro4Character(); // 생성자

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WeaponMode CurrentWeaponMode; // 현재 장착한 무기 유형

	enum class CharacterState // 캐릭터 위상 상태
	{
		Standing,
		Crouching,
	};

	CharacterState CurrentCharacterState; // 현재 캐릭터 위상

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 사용자 입력을 받는 함수

	// 캐릭터 장착아이템 설정(무기, 방어구)
	void SetPlayerWeapon(class AAWeapon* SetWeapon);
	void SetPlayerArmor(class AAArmor* Armor);
	void AddPlayerGrenade(class AAGrenade* _Grenade);
	void SetPlayerRound(class AAmmo* _Ammo);
	void EquipGrenade();

	UFUNCTION(Client, Reliable)
	void FlashBangExplosion();

	/* 좀비 스포너를 탐지하는 콜리전을 활성화하기 위한 함수 */
	void DetectZombieSpawner(bool isNight); 
	
	UFUNCTION(BlueprintCallable)
	void DrawPunch();

	// 잠식 시스템 함수
	void StartEncroachTimer();
	UFUNCTION(Server, Reliable)
	void SetPlayerEncroach();
	void StopEncroachTimer();
	UFUNCTION(Server, Reliable)
	void RecoveryEncroach();

	void PlayerEscape(); //
	
	APro4PlayerController* GetPlayerController();
	void SetPlayerController(APro4PlayerController* PlayerController);

	/* 카메라 변수 */
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
	/* 카메라 변수 */

	/* 총알 변수 */
	UPROPERTY(EditAnywhere, Category = "GamePlay")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<APro4Projectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	UParticleSystemComponent* MuzzleFlash;
	/* 총알 변수 */
		
	/* 무기, 방어구 */
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
	/* 무기, 방어구 */

	
	// 탈출 조건 달성 확인
	bool GetIsPossibleEscape()
	{
		return IsPossibleEscape;
	}

	UFUNCTION(Server, Reliable)
	void SetIsPossibleEscapeOnServer(bool Escape);

	/* 다른 클래스에서 캐릭터 상태 확인과 수정을 위한 함수들 */
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
	/* 다른 클래스에서 캐릭터 상태 확인과 수정을 위한 함수들 */

	/* 캐릭터와 카메라 회전값을 애니메이션에 반영하기 위한 함수*/
	float CharacterPitch()
	{
		return CharacterRotationPitch;
	}

	float CharacterArmPitch()
	{
		return CharacterArmControl;
	}
	/* 캐릭터와 카메라 회전값을 애니메이션에 반영하기 위한 함수*/

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

	/* 움직이는 방향을 애니메이션에 반영하기위한 함수*/
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
	/* 움직이는 방향을 애니메이션에 반영하기위한 함수*/

	/* 잠식 여부를 확인하기 위한 함수 */
	void Encroached()
	{
		IsEncroach = true;
	}

	void UnEncroached()
	{
		IsEncroach = false;
	}
	/* 잠식 여부를 확인하기 위한 함수 */

	/* Helicopter */
	UFUNCTION(Server, Reliable)
	void CallHelicopterToEscapeOnServer();

	/* 아이템 획득 시, 해당 아이템을 공통적으로 제거하는 함수 */
	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* DestroyActor);

	/* 수류탄을 던지는 타이밍에 실행되는 함수 */
	UFUNCTION()
	void ThrowGrenade();

#pragma region PlayerState
public:
	/* 캐릭터 체력, 방어력 */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxHP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
	float MaxAP;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category=State)
	float CurrentAP;
	/* 캐릭터 체력, 방어력 */

	/* 서버에서 캐릭터 체력 회복 정보를 갱신하기 위한 함수 */
	UFUNCTION(Server, Reliable, WithValidation)
	void RecoverPlayerHealthOnServer();

	/* 서버에서 캐릭터 피격 정보를 갱신하기 위한 함수 */
	UFUNCTION(Server, Reliable, WithValidation)
	void PlayerHealthGetDamagedOnServer(float Damage, AActor* AttackActor);

	// 캐릭터 피격
	void GetDamaged(float Damage, AActor* AttackActor);

	/* 플레이어의 킬 정보를 저장하는 함수 */
	UFUNCTION(Server, Reliable)
	void RecordPlayerKill(AActor* AttackActor);

	UFUNCTION(Client, Reliable)
	void GameOver();

	/* 공격 효과음 */
	class UAudioComponent* FireA;
	class USoundCue* SRSound;
	class USoundCue* SubS;
	class USoundCue* EmptyS;
	class USoundCue* FireS;
	/* 공격 효과음 */

private:
	// 캐릭터 피격 여부
	UPROPERTY(Replicated)
	bool bIsPlayerGetAttacked = false;

	/* 캐릭터 체력 회복 변수*/
	UPROPERTY(Replicated)
	bool bIsRecoveryTimerStarted = false;

	FTimerHandle HealthRecoveryTimer;
	FTimerDelegate HealthRecoveryDelegate;
	/* 캐릭터 체력 회복 변수*/

	/* 캐릭터 방어구 정보를 담은 구조체 변수 */
	FArmorInfo PlayerHelmet;
	FArmorInfo PlayerVest;
	/* 캐릭터 방어구 정보를 담은 구조체 변수 */

	/* 캐릭터 무기 정보를 담은 구조체 변수 */
	FWeaponInfo MainWeapon;
	FWeaponInfo SubWeapon;
	FWeaponInfo Knife;

	FGrenadeInfo PlayerGrenade;
	/* 캐릭터 무기 정보를 담은 구조체 변수 */

	class UNecrophobiaGameInstance* NecGameInstance;
#pragma endregion

private:
	void NotifyActorBeginOverlap(AActor* Act) override;
	void NotifyActorEndOverlap(AActor* Act) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHeli_AH64D> BP_Helicopter;

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
	void Jump();
	float UpdownSpeed();
	float LeftRightSpeed();
	void ViewPoint();

	// 공격 관련 함수
	void Zoom();
	void Attack();
	void StartFire();
	void StopFire();
	void Fire();
	void Fire_Mod();
	void Throw();
	void Punch();
	void Stab();

	// 장착 함수
	void EquipMain();
	void EquipSub();
	void EquipKnife();
	void EquipATW();
	void Reload();
	void InteractPressed();

	/* 플레이어 UI 교체 함수 */
	void ChangePlayerWidget();

	float CameraRotationX;

	// 상태플래그
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

	// 애니메이션 컨트롤을 위한 플래그
	float HoldTime;
	float MoveRate;
	int32 HoldFlag;
	UPROPERTY(Replicated)
	int32 Moveflag;

	// 잠식 관련 변수
	float EncroachTime;
	FTimerHandle EncroachTimer;
	int32 EncroachLevel = 0;
	bool IsEncroach;

	// 캐릭터 애니메이션 컨트롤에 사용할 회전값 저장 변수
	float CharacterRotationPitch;
	float CharacterArmControl;

	// 연사속도 조절 변수
	FTimerHandle FireDelay;

	

	/* 애니메이션 몽타주 종료시 콜백되는 함수 */
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
	/* 애니메이션 몽타주 종료시 콜백되는 함수 */

	/* 애니메이션 몽타주 작동중인지 체크하기 위한 변수 */
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
	/* 애니메이션 몽타주 작동중인지 체크하기 위한 변수 */

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float temp;

	// 애니메이션 연결을 위한 클래스 변수
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

	// 현재 스폰된 좀비수와 최대 좀비 수
	UPROPERTY(Replicated)
	uint16 SpawnZombieCurCount = 0;
	uint16 SpawnZombieMaxCount = 20;

	/* 좀비 스포너 */
	UFUNCTION()
	void ZombieSpawnerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ZombieSpawnerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/* 좀비 스포너 */

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

	/* 플레이어가 탈출 조건에 적절했을 때  */
	UFUNCTION(Server, Reliable)
	void SuccessPlayerEscapeOnServer();

	UFUNCTION(Client, Reliable)
	void SuccessPlayerEscapeOnClient();

	/* 플레이어 미니맵 세팅 */
	UFUNCTION(Server, Reliable)
	void SetTextureTargetOnServer();

	UFUNCTION(Client, Reliable)
	void SetTextureTargetOnClient();

	UPaperSprite* RenderIcon;

	UTextureRenderTarget2D* RenderTarget;
};