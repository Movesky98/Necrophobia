// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Character.h"
#include "Pro4AnimInstance.h"
#include "NecrophobiaGameInstance.h"
#include "UserInterface/PlayerMenu.h"
#include "Item/AWeapon.h"
#include "Item/AArmor.h"
#include "Item/AGrenade.h"
#include "Item/Ammo.h"
#include "InGamePlayerState.h"

#include "ZombieSpawner.h"
#include "Heli_AH64D.h"
#include "Door.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APro4Character::APro4Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;
	NetCullDistanceSquared = 9000000202358128640.0f;
 
	/* 캐릭터 클래스를 구성하는 컴포넌트(카메라, 방어구, 무기..) */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HELMET"));
	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VEST"));
	Scope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SCOPE"));
	Grenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GRENADE"));
	DetectZSpawnerCol = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectCollsion"));
	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
	MapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MAPSPRINGARM"));
	MapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MAPCAPTURE"));
	/* 캐릭터 클래스를 구성하는 컴포넌트(카메라, 방어구, 무기..) */

	/* 총알 발사효과를 무기컴포넌트의 하위로*/
	MuzzleFlash->SetupAttachment(Weapon);
	MuzzleFlash->bAutoActivate = false;
	/* 총알 발사효과를 무기컴포넌트의 하위로*/

	/* 발사효과로 사용할 파티클 지정 */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashAsset(TEXT("/Game/Impacts/Particles/MuzzleFlash/P_MuzzleFlash_3"));
	if (MuzzleFlashAsset.Succeeded())
	{
		MuzzleFlash->SetTemplate(MuzzleFlashAsset.Object);
	}

	/* 컴포넌트 계층 설정 */
	RootComponent = GetCapsuleComponent();
	DetectZSpawnerCol->SetupAttachment(GetCapsuleComponent());
	DetectZSpawnerCol->SetIsReplicated(true);
	DetectZSpawnerCol->SetBoxExtent(DetectExtent);
	DetectZSpawnerCol->SetCollisionProfileName(TEXT("Detect_ZSpawner"));
	DetectZSpawnerCol->SetGenerateOverlapEvents(false);

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	MapSpringArm->SetupAttachment(GetCapsuleComponent());
	MapCapture->SetupAttachment(MapSpringArm);
	MapSpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	/* 컴포넌트 계층 설정 */

	/* 캐릭터 메쉬 위치, 회전 값 설정*/
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PCharacter"));

	/* 초기설정 함수 */
	CameraSetting();
	MovementSetting();
	WeaponSetting();
	StateSetting();
	/* 초기설정 함수 */

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Mannequin(TEXT("/Game/Character_Animation/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>SK_ANIM(TEXT("/Game/Character_Animation/Mannequin/Animations/UE4AnimBluprint.UE4AnimBluprint_C"));
	if (SK_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SK_ANIM.Class);
	}

	/* Helicopter 클래스를 찾아옴 */
	static ConstructorHelpers::FObjectFinder<UBlueprint> Helicopter(TEXT("/Game/VigilanteContent/Vehicles/West_Heli_AH64D/BP_TESTHeli"));
	if(Helicopter.Succeeded())
	{
		BP_Helicopter = Helicopter.Object;
	}

	SocketSetting();

	Tags.Add("Player");

	static ConstructorHelpers::FObjectFinder<USoundCue>FireSound(TEXT("SoundCue'/Game/StarterContent/Audio/ShootSound.ShootSound'"));
	FireS = FireSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue>SubSound(TEXT("SoundCue'/Game/StarterContent/Audio/SubShoots.SubShoots'"));
	SubS = SubSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue>EmptySound(TEXT("SoundCue'/Game/StarterContent/Audio/EmptyShoots.EmptyShoots'"));
	EmptyS = EmptySound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue>SRSounds(TEXT("SoundCue'/Game/StarterContent/Audio/SRSounds.SRSounds'"));
	SRSound = SRSounds.Object;
	FireA = CreateDefaultSubobject<UAudioComponent>(TEXT("FireA"));
	FireA->bAutoActivate = false;
	FireA->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void APro4Character::BeginPlay()
{
	Super::BeginPlay();
	
	DetectZSpawnerCol->OnComponentBeginOverlap.AddDynamic(this, &APro4Character::ZombieSpawnerBeginOverlap);
	DetectZSpawnerCol->OnComponentEndOverlap.AddDynamic(this, &APro4Character::ZombieSpawnerEndOverlap);
	NecGameInstance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, GetActorLocation().ToString());

	PlayerController = Cast<APro4PlayerController>(GetWorld()->GetFirstPlayerController());
}

/// <summary>
////////////////////////////////////////////////////// 초기세팅 ////////////////////////////////////////////////////////////
/// </summary>

// 카메라 초기 세팅
void APro4Character::CameraSetting()
{
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	SpringArm->SocketOffset = FVector(0.0f, 50.0f, 100.0f);

	MapSpringArm->bInheritPitch = true;
	MapSpringArm->bInheritRoll = true;
	MapSpringArm->bInheritYaw = true;

	MapCapture->ProjectionType = ECameraProjectionMode::Perspective;
	MapCapture->OrthoWidth = 1000.0f;
}

// 캐릭터 위상 세팅
void APro4Character::MovementSetting()
{
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	CurrentCharacterState = CharacterState::Standing;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	IsRun = false;
	IsHold = false;
	IsZoom = false;
	IsForward = true;
	PlayerRun = false;
	Moveflag = 0;
	Updownflag=0;
	LeftRightflag=0;
}

// 캐릭터 무기정보 세팅
void APro4Character::WeaponSetting()
{
	ProjectileClass = APro4Projectile::StaticClass();
	Equipflag = 0;
	IsEquipping = false;
	IsReloading = false;
	FireMod = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/Weapon/Granade/granade"));
	if (SM_Grenade.Succeeded())
	{
		PlayerGrenade.SM_Grenade = SM_Grenade.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Flash(TEXT("/Game/Weapon/Granade/flashbang"));
	if (SM_Flash.Succeeded())
	{
		PlayerGrenade.SM_Flash = SM_Flash.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Smoke(TEXT("/Game/Weapon/Granade/smoke"));
	if (SM_Smoke.Succeeded())
	{
		PlayerGrenade.SM_Smoke = SM_Smoke.Object;
	}
}

// 캐릭터 상태 세팅
void APro4Character::StateSetting()
{
	MaxHP = 100.0f;
	CurrentHP = 100.0f;
	MaxAP = 100.0f;
	CurrentAP = 0.0f;

	MainWeapon.Magazine = 30;
	MainWeapon.TotalRound = 0;
	SubWeapon.Magazine = 25;
	SubWeapon.TotalRound = 0;
	Knife.Magazine = 0;
	Knife.TotalRound = 0;

	CurrentWeaponMode = WeaponMode::Disarming;
	CurrentCharacterState = CharacterState::Standing;

	HoldTime = 0.0f;
	HoldFlag = 0;

	EncroachLevel = 0;
	IsEncroach = false;
	EncroachTime = 0.0f;

	CanZoom = true;
}

// 무기, 총알, 장비 장착지점 세팅
void APro4Character::SocketSetting()
{
	FName WeaponSocket(TEXT("Hand_rSocket"));
	FName GrenadeSocket(TEXT("Hand_r_GrenadeSocket"));
	FName MuzzleFlashSocket(TEXT("gunFireLocation"));
	FName HeadSocket(TEXT("headSocket"));
	FName VestSocket(TEXT("VestSocket"));

	if (GetMesh()->DoesSocketExist(WeaponSocket)) {

		UE_LOG(Pro4, Warning, TEXT("WeaponSocket has exist."));

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		Weapon->SetIsReplicated(true);
	}
	else
	{
		UE_LOG(Pro4, Error, TEXT("WeaponSocket has not exist."));
	}

	if (GetMesh()->DoesSocketExist(HeadSocket)) {
		UE_LOG(Pro4, Warning, TEXT("HeadSocket has exist."));

		Helmet->SetupAttachment(GetMesh(), HeadSocket);
		Helmet->SetIsReplicated(true);
	}
	else
	{
		UE_LOG(Pro4, Error, TEXT("HeadSocket has not exist."));
	}

	if (GetMesh()->DoesSocketExist(VestSocket)) {
		UE_LOG(Pro4, Warning, TEXT("VestSocket has exist."));

		Vest->SetupAttachment(GetMesh(), VestSocket);
		Vest->SetIsReplicated(true);
	}
	else
	{
		UE_LOG(Pro4, Error, TEXT("VestSocket has not exist."));
	}

	if (GetMesh()->DoesSocketExist(GrenadeSocket)) {
		UE_LOG(Pro4, Warning, TEXT("GrenadeSocket has exist."));

		Grenade->SetupAttachment(GetMesh(), GrenadeSocket);
		Grenade->SetRelativeScale3D(FVector(2.0f));
		Grenade->SetIsReplicated(true);
	}
	else
	{
		UE_LOG(Pro4, Error, TEXT("GrenadeSocket has not exist."));
	}
}

/// <summary>
////////////////////////////////////////////////////// 초기세팅 ////////////////////////////////////////////////////////////
/// </summary>

// Called every frame
void APro4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 위상 변화 애니메이션 작동 중 다른행동을 막기위한 부분
	if (IsHold)
	{
		HoldTime += DeltaTime;
		if (HoldTime >= 0.3f)
		{
			IsHold = false;
			HoldTime = 0.0f;
			HoldFlag = 0;
			CanZoom = true;
		}
	}

	// 점프 상태가 아닐 때 줌 가능하도록
	if (!GetMovementComponent()->IsFalling())
	{
		CanZoom = true;
	}

	// 대각선 이동을 위한 세팅
	if (Updownflag == 1 && LeftRightflag != 0)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	}
	else
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	}
	
	
	CharacterRotationPitch = GetControlRotation().Pitch;
	if (IsZoom)
	{
		CharacterArmControl= GetControlRotation().Pitch;
	}
	else
	{
		CharacterArmControl = 0.0f;
	}
	// Character Role Test.
	// DrawDebugString(GetWorld(), FVector(0, 0, 150), GetEnumRole(GetLocalRole()), this, FColor::Green, DeltaTime);
}

// Character Role Test.
FString APro4Character::GetEnumRole(ENetRole CharacterRole)
{
	switch (CharacterRole)
	{
	case ROLE_None:
		return "None";
		break;
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
		break;
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
		break;
	case ROLE_Authority:
		return "Authority";
		break;
	default:
		return "ERROR";
		break;
	}
}

// 몽타주 중복실행을 막기위한 함수
void APro4Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Pro4Anim = Cast<UPro4AnimInstance>(GetMesh()->GetAnimInstance());

	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnEquipMontageEnded);
	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnReloadMontageEnded);
	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnAttackMontageEnded);
}

// 장착 몽타주 종료시 콜백
void APro4Character::OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsEquipping = false;
	CanZoom = true;
}

// 장전 몽타주 종료시 콜백
void APro4Character::OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsReloading = false;
	CanZoom = true;
}

// 공격 몽타주 종료시 콜백
void APro4Character::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsAttacking = false;
}

// Called to bind functionality to input
void APro4Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &APro4Character::StopFire); // ��� ����
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APro4Character::StartFire); // ����
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &APro4Character::Zoom);
	PlayerInputComponent->BindAction(TEXT("FireMod"), EInputEvent::IE_Pressed, this, &APro4Character::Fire_Mod);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APro4Character::Jump);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APro4Character::beCrouch);
	PlayerInputComponent->BindAction(TEXT("Key1"), EInputEvent::IE_Pressed, this, &APro4Character::EquipMain);
	PlayerInputComponent->BindAction(TEXT("Key2"), EInputEvent::IE_Pressed, this, &APro4Character::EquipSub);
	PlayerInputComponent->BindAction(TEXT("Key3"), EInputEvent::IE_Pressed, this, &APro4Character::EquipKnife);
	PlayerInputComponent->BindAction(TEXT("Key4"), EInputEvent::IE_Pressed, this, &APro4Character::EquipATW);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &APro4Character::Run);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APro4Character::Reload);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APro4Character::InteractPressed);
	PlayerInputComponent->BindAction(TEXT("ChangeWidget"), EInputEvent::IE_Pressed, this, &APro4Character::ChangePlayerWidget);
	
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APro4Character::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APro4Character::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APro4Character::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APro4Character::Turn);
}

/// <summary>
////////////////////////////////////////////////////// 캐릭터 움직임 코드 ////////////////////////////////////////////////////////////
/// </summary>

// 앞 뒤 이동시 속도 조정
float APro4Character::UpdownSpeed()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Standing:
		if (Updownflag == 1)
		{
			if (PlayerRun)
				SetPlayerStateOnServer("Run", true);

			if (IsRun)
			{
				return 1.0f;
			}
			else
			{
				return 0.5f;
			}
		}
		else
		{
			if(IsRun)
				SetPlayerStateOnServer("Run", false);
			return 0.3f;
		}
		break;
	case CharacterState::Crouching:
		if (Updownflag == 1)
		{
			return 0.3f;
		}
		else
		{
			return 0.3f;
		}
		break;
	default:
		return 0.0f;
	}
}

// 좌우 이동시 속도 조정
float APro4Character::LeftRightSpeed()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Standing:
		SetPlayerStateOnServer("Run", false);
		if (PlayerRun)
		{
			if (LeftRightflag == 1)
			{
				return 0.5f;
			}
			else
			{
				return 0.5f;
			}
		}
		else
		{
			if (LeftRightflag == 1)
			{
				return 0.3f;
			}
			else
			{
				return 0.3f;
			}
		}
		break;
	case CharacterState::Crouching:
		if (Updownflag == 1)
		{
			return 0.3f;
		}
		else
		{
			return 0.3f;
		}
		break;
	default:
		return 0.0f;
	}
}

// 앞 뒤 이동
void APro4Character::UpDown(float NewAxisValue)
{
	if (!IsHold)
	{
		if (NewAxisValue > 0.5f || NewAxisValue < -0.5f)
		{
			IsForward = true;
			if (NewAxisValue < 0)
			{
				SetPlayerFlagOnServer("MoveFlag", 1);
				Updownflag = -1;
			}
			else
			{
				SetPlayerFlagOnServer("MoveFlag", 0);
				Updownflag = 1;
			}
		}
		else
		{
			IsForward = false;
			Updownflag = 0;
		}

		MoveRate = NewAxisValue * UpdownSpeed();
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), MoveRate);
	}
}

// 좌 우 이동
void APro4Character::LeftRight(float NewAxisValue)
{
	if (!IsHold)
	{
		if (NewAxisValue > 0.5f || NewAxisValue < -0.5f)
		{
			IsForward = false;
			if (NewAxisValue < 0)
			{
				SetPlayerFlagOnServer("MoveFlag", 1);
				LeftRightflag = 1;
			}
			else
			{
				SetPlayerFlagOnServer("MoveFlag", 0);
				LeftRightflag = -1;
			}
		}
		else
		{
			LeftRightflag = 0;
		}
		MoveRate = NewAxisValue * LeftRightSpeed();
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), MoveRate);
	}
}

// 위아래 시점 회전
void APro4Character::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

// 좌우 시점 회전
void APro4Character::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

// 달리기
void APro4Character::Run()
{
	if (CurrentCharacterState == CharacterState::Standing)
		SetPlayerStateOnServer("Run", !IsRun);

	PlayerRun = !PlayerRun;

	if (IsZoom)
		Zoom();
}

// 점프
void APro4Character::Jump()
{
	if (!IsHold)
	{
		switch (CurrentCharacterState)
		{
		case CharacterState::Standing:
			Super::Jump();
			break;
		case CharacterState::Crouching:
			HoldFlag = 1;
			Super::UnCrouch();
			CurrentCharacterState = CharacterState::Standing;
			break;
		default:
			break;
		}

		if (IsZoom)
			Zoom();

		CanZoom = false;
	}

}

// 앉기
void APro4Character::beCrouch()
{
	if (!IsHold)
	{
		IsHold = true;
		if (!GetMovementComponent()->IsFalling())
		{
			switch (CurrentCharacterState)
			{
			case CharacterState::Standing:
				Super::Crouch();
				HoldFlag = 1;
				CurrentCharacterState = CharacterState::Crouching;
				break;
			case CharacterState::Crouching:
				Super::UnCrouch();
				HoldFlag = 1;
				CurrentCharacterState = CharacterState::Standing;
				break;
			default:
				break;
			}
		}

		if (IsZoom)
			Zoom();

		CanZoom = false;
	}
}
/// <summary>
////////////////////////////////////////////////////// 캐릭터 움직임 코드 ////////////////////////////////////////////////////////////
/// </summary>

/// <summary>
////////////////////////////////////////////////////// 캐릭터 무기장착, 장전 코드 ////////////////////////////////////////////////////////////
/// </summary>

// 1번 무기
void APro4Character::EquipMain()
{
	// 메인무기 보유시 장착 가능
	if (MainWeapon.bHaveWeapon)
	{
		// 다른 몽타주 실행중이라면 해당 몽타주 종료
		if (IsMontagePlay)
		{
			Pro4Anim->Montage_Stop(0.0f);
			IsMontagePlay = false;
		}

		// 무기 장착
		if (CurrentWeaponMode == WeaponMode::Main)
		{
			SetPlayerFlagOnServer("EquipFlag", 0);
			CurrentWeaponMode = WeaponMode::Disarming;
		}
		else
		{
			if (IsZoom)
				Zoom();

			SetPlayerFlagOnServer("EquipFlag", 1);
			PlayMontageOnServer(Pro4Anim->GetEquipMontage(), 1);
			IsMontagePlay = true;
			IsEquipping = true;
			CurrentWeaponMode = WeaponMode::Main;
		}
	}

	EquipPlayerWeaponOnServer(CurrentWeaponMode);
}

// 2번 무기
void APro4Character::EquipSub()
{
	// 보조무기 보유시 장착 가능
	if (SubWeapon.bHaveWeapon)
	{
		// 다른 몽타주 실행중이라면 해당 몽타주 종료
		if (IsMontagePlay)
		{
			Pro4Anim->Montage_Stop(0.0f);
			IsMontagePlay = false;
		}

		// 무기 장착
		if (CurrentWeaponMode == WeaponMode::Sub)
		{
			SetPlayerFlagOnServer("EquipFlag", 0);
			CurrentWeaponMode = WeaponMode::Disarming;
		}
		else
		{
			if (IsZoom)
				Zoom();

			SetPlayerFlagOnServer("EquipFlag", 1);
			PlayMontageOnServer(Pro4Anim->GetEquipMontage(), 2);
			IsMontagePlay = true;
			IsEquipping = true;
			CurrentWeaponMode = WeaponMode::Sub;
		}
	}

	EquipPlayerWeaponOnServer(CurrentWeaponMode);
}

// 보조 무기
void APro4Character::EquipKnife()
{
	// 칼 보유중일시 장착 가능
	if (Knife.bHaveWeapon)
	{
		// 다른 몽타주 실행중이라면 해당 몽타주 종료
		if (IsMontagePlay)
		{
			Pro4Anim->Montage_Stop(0.0f);
			IsMontagePlay = false;
		}

		// 무기 장착
		if (CurrentWeaponMode == WeaponMode::Knife)
		{
			SetPlayerFlagOnServer("EquipFlag", 0);
			CurrentWeaponMode = WeaponMode::Disarming;
		}
		else
		{
			if (IsZoom)
				Zoom();

			SetPlayerFlagOnServer("EquipFlag", 2);
			PlayMontageOnServer(Pro4Anim->GetEquipMontage(), 2);
			IsMontagePlay = true;
			IsEquipping = true;
			CurrentWeaponMode = WeaponMode::Knife;
		}
	}

	EquipPlayerWeaponOnServer(CurrentWeaponMode);
}

// 투척 무기
void APro4Character::EquipATW()
{
	EquipGrenade();

	UStaticMesh* EquipGrenadeMesh = nullptr;

	if (PlayerGrenade.EquipGrenade == "Grenade")
	{
		EquipGrenadeMesh =  PlayerGrenade.SM_Grenade;
	}
	else if (PlayerGrenade.EquipGrenade == "Smoke")
	{
		EquipGrenadeMesh = PlayerGrenade.SM_Smoke;
	}
	else if (PlayerGrenade.EquipGrenade == "Flash")
	{
		EquipGrenadeMesh = PlayerGrenade.SM_Flash;
	}

	EquipPlayerWeaponOnServer(CurrentWeaponMode, EquipGrenadeMesh);
}

// 장전
void APro4Character::Reload()
{
	// 이미 장전중이거나 다른 몽타주 실행중일시 중단
	if (IsReloading)
		return;
	if (!IsMontagePlay)
	{
		if (IsZoom)
			Zoom();
		// 무기 유형마다 다른 애니메이션, 각각 변수 설정
		switch (CurrentWeaponMode)
		{
		// 주무기 장전
		case WeaponMode::Main:
			if (CurrentCharacterState == CharacterState::Standing)
			{
				PlayMontageOnServer(Pro4Anim->GetReloadMontage(), 1);
				IsMontagePlay = true;
				IsReloading = true;
			}
			else if (CurrentCharacterState == CharacterState::Crouching)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}

			// 현재 가지고 있는 탄약 수 = 현재 가지고 있는 탄약 수 - (주무기의 탄창에 들어갈 수 있는 탄약 수 - 현재 탄창에 들어가있는 탄약 수)
			if (MainWeapon.TotalRound <= MainWeapon.Magazine)
			{
				MainWeapon.CurrentRound = MainWeapon.TotalRound;
				MainWeapon.TotalRound = 0;
			}
			else
			{
				MainWeapon.TotalRound -= MainWeapon.Magazine - MainWeapon.CurrentRound;
				MainWeapon.CurrentRound = MainWeapon.Magazine;
			}
			break;
		// 보조무기 장전
		case WeaponMode::Sub:
			if (CurrentCharacterState == CharacterState::Standing)
			{
				PlayMontageOnServer(Pro4Anim->GetReloadMontage(), 2);
				IsMontagePlay = true;
				IsReloading = true;
			}
			else
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}


			// 현재 가지고 있는 탄약 수 = 현재 가지고 있는 탄약 수 - (주무기의 탄창에 들어갈 수 있는 탄약 수 - 현재 탄창에 들어가있는 탄약 수)
			if (SubWeapon.TotalRound <= SubWeapon.Magazine)
			{
				SubWeapon.CurrentRound = SubWeapon.TotalRound;
				SubWeapon.TotalRound = 0;
			}
			else
			{
				SubWeapon.TotalRound -= SubWeapon.Magazine - SubWeapon.CurrentRound;
				SubWeapon.CurrentRound = SubWeapon.Magazine;
			}
			break;
		default:
			break;
		}
	}
}
/// <summary>
////////////////////////////////////////////////////// 캐릭터 무기장착, 장전 코드 ////////////////////////////////////////////////////////////
/// </summary>

#pragma region Character_Attack

// 공격
void APro4Character::Attack()
{
	if (!IsEquipping)
	{
		// 무기마다 다른 공격 모션
		switch (CurrentWeaponMode)
		{
		// 총은 Fire
		case WeaponMode::Main:
			IsFire = true;
			Fire();
			break;
		case WeaponMode::Sub:
			IsFire = true;
			Fire();
			break;
		// 칼은 Swing
		case WeaponMode::Knife:
			break;
		// 투척무기는 Throw
		case WeaponMode::ATW:
			Throw();
			break;
		// 비무장은 Punch
		case WeaponMode::Disarming:
			IsPunch = true;
			Punch();
			break;
		}
	}
}

// 스코프 줌 인,아웃
void APro4Character::Zoom()
{
	if (Weapon != nullptr && CanZoom == true && !IsEquipping && !IsReloading)
	{
		// 장착 무기가 총기류일때 줌 가능
		if (CurrentWeaponMode == WeaponMode::Main || CurrentWeaponMode == WeaponMode::Sub)
		{
			// 줌 인 가능한 상태일시 카메라 위치 설정과 스코프 종류에 따라 확대
			if (IsZoom)
			{
				SetPlayerStateOnServer("Zoom", false);
				SpringArm->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				SpringArm->TargetArmLength = 450.0f;
				SpringArm->SocketOffset = FVector(0.0f, 100.0f, 50.0f);
				Camera->FieldOfView = 90.0f;
			}
			else
			{
				SetPlayerStateOnServer("Zoom", true);
				SpringArm->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetIncludingScale, "b_gun_scopeCamera");
				SpringArm->TargetArmLength = 0.0f;
				SpringArm->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
				if (MainWeapon.Name == "SR")
				{
					Camera->FieldOfView = 22.5f;
					Camera->PostProcessSettings.VignetteIntensity = 1.0f;
				}
				// 달리기 상태였을시 해제
				if (IsRun)
					SetPlayerStateOnServer("Run", false);
			}

			// 장착무기가 SR일시 스코프 UI로 변경
			if (MainWeapon.Name == "SR")
			{
				UNecrophobiaGameInstance* Instance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
				Instance->PlayerMenu->PlayerZoomWidget();
			}
		}
	}
}

// 총기 조종간
void APro4Character::Fire_Mod()
{
	if ((CurrentWeaponMode == WeaponMode::Main && MainWeapon.Name == "AR") || CurrentWeaponMode == WeaponMode::Sub)
		FireMod = !FireMod;
}

// 마우스 클릭시 실행
void APro4Character::StartFire()
{
	Attack();
}

// 마우스에서 때면 실행
void APro4Character::StopFire()
{
	if (FireMod)
	{
		FireA->Stop();
	}
	IsFire = false;
	IsPunch = false;
}

// 총 발사
void APro4Character::Fire()
{
	if (IsFire)
	{
		FVector MuzzleLocation;
		FRotator MuzzleRotation;
		// 무기 장착중일 때 총알 스폰 지점 설정
		if (Weapon != nullptr)
		{
			if (Weapon->DoesSocketExist("gunFireLocation"))
			{
				FTransform SocketTransform;
				MuzzleLocation = Weapon->GetSocketLocation("gunFireLocation");
				MuzzleRotation = Weapon->GetSocketRotation("gunFireLocation");
			}

		}

		if (CurrentWeaponMode == WeaponMode::Main) // 주무기일 때의 총알 발사 (탄창 상태 반영 안함)
		{
			if (MainWeapon.CurrentRound <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("There is no bullet."));
				FireA->SetSound(EmptyS);
				FireA->Play();
				Reload();
				return;
			}
			else
			{
				if (MainWeapon.Name == "SR")
				{
					FireA->SetSound(SRSound);
				}
				else
				{
					FireA->SetSound(FireS);
				}
				FireA->Play();
			}

			MainWeapon.CurrentRound--;
		}
		else if (CurrentWeaponMode == WeaponMode::Sub) // 보조무기일 때의 총알 발사
		{
			if (SubWeapon.CurrentRound <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("There is no bullet."));
				FireA->SetSound(EmptyS);
				FireA->Play();
				Reload();
				return;
			}
			else
			{
				FireA->SetSound(SubS);
				FireA->Play();
			}

			SubWeapon.CurrentRound--;
		}

		// 총알 발사 애니메이션
		if (!IsMontagePlay)
		{
			if (IsZoom)
			{
				PlayMontageOnServer(Pro4Anim->GetAttackMontage(), 2);
				IsMontagePlay = true;
				IsAttacking = true;
				UE_LOG(Pro4, Log, TEXT("2"));
			}
			else
			{
				PlayMontageOnServer(Pro4Anim->GetAttackMontage(), 1);
				IsMontagePlay = true;
				IsAttacking = true;
				UE_LOG(Pro4, Log, TEXT("1"));
			}
		}

		MuzzleFlash->ToggleActive();
		SpawnProjectileOnServer(MuzzleLocation, MuzzleRotation, MuzzleRotation.Vector(), this);
		
		if (FireMod) // 연사 상태이면 함수 딜레이 후 다시 콜 (주무기 종류에 따라서 연사속도 변경)
		{
			GetWorld()->GetTimerManager().SetTimer(FireDelay, this, &APro4Character::Fire, .075f, false);
		}
	}
}

// 투척무기 던지기
void APro4Character::Throw()
{
	/*
	* 던지는 애니메이션
	*/
	UE_LOG(Pro4, Log, TEXT("ATW Throw"));

	// 투척무기 보유시 장착중인 투척무기 투척
	if (Grenade->GetStaticMesh() != nullptr)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector ThrowLocation;

		if (GetMesh()->DoesSocketExist("Hand_r_GrenadeSocket"))
		{
			ThrowLocation = GetMesh()->GetSocketLocation("Hand_r_GrenadeSocket");
			ThrowLocation += GetActorForwardVector() * 200.0f;
		}

		FRotator ThrowRotation = CameraRotation;
		ThrowRotation.Pitch += 10.0f;

		DrawDebugSolidBox(GetWorld(), ThrowLocation, FVector(5.0f), FColor::Blue, true, 5.0f);
		SpawnGrenadeOnServer(ThrowLocation, ThrowRotation, ThrowRotation.Vector(), this);
	}
}

void APro4Character::Punch() // 주먹질
{
	/* 주먹질 애니메이션 꾹 눌렀을 때 주먹질 계속하도록 */
	if (!IsMontagePlay)
	{
		PlayMontageOnServer(Pro4Anim->GetPunchMontage(), 1);
		IsMontagePlay = true;
		IsAttacking = true;
	}
}

/* 플레이어가 서버에게 총알을 스폰해달라고 요청하는 함수 */
void APro4Character::SpawnProjectileOnServer_Implementation(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = _Owner;
		SpawnParams.Instigator = GetInstigator();

		// 총알 생성 후 발사 방향과 속도
		APro4Projectile* Projectile = World->SpawnActor<APro4Projectile>(ProjectileClass, Location, Rotation, SpawnParams);
		if (Projectile)
		{
			Projectile->FireInDirection(LaunchDirection);
		}
	}
}

bool APro4Character::SpawnProjectileOnServer_Validate(FVector Location, FRotator Rotator, FVector LaunchDirection, AActor* _Owner)
{
	return true;
}

/* 플레이어가 서버에게 수류탄을 스폰해달라고 요청하는 함수 */
void APro4Character::SpawnGrenadeOnServer_Implementation(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Server spawn Grenade"));
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = _Owner;
		SpawnParams.Instigator = GetInstigator();

		// 수류탄 생성 후 투척 물리 설정
		AAGrenade* SpawnGrenade = World->SpawnActor<AAGrenade>(AAGrenade::StaticClass(), Location, Rotation, SpawnParams);

		if (SpawnGrenade)
		{
			SpawnGrenade->ThrowGrenade(PlayerGrenade.EquipGrenade, Grenade->GetStaticMesh());
			SpawnGrenade->SetSimulatePhysics(LaunchDirection);
		}
	}
}

bool APro4Character::SpawnGrenadeOnServer_Validate(FVector Location, FRotator Rotation, FVector LaunchDirection, AActor* _Owner)
{
	return true;
}

#pragma endregion

/* F 버튼을 눌렀을 때 아이템, 문 등 상호작용을 하기위한 함수 */
void APro4Character::InteractPressed()
{
	UE_LOG(Pro4, Log, TEXT("Interact Pressed."));

	FVector CharacterLoc;
	FRotator CharacterRot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(CharacterLoc, CharacterRot);

	FVector Start = CharacterLoc;
	FVector End = CharacterLoc + (CharacterRot.Vector() * 1000);

	FCollisionQueryParams TraceParams;
	UWorld* World = GetWorld();

	bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// 라인트레이싱을 통해 해당지점에 있는 사물 정보 획득
	if (bHit)
	{
		if (Hit.GetActor()) {
			DrawDebugSolidBox(World, Hit.ImpactPoint, FVector(5.0f), FColor::Blue, false, 2.0f);
			DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Red, false, 2.0f);

			UE_LOG(Pro4, Log, TEXT("HitActor : %s"), *Hit.GetActor()->GetName());

			AActor* Interactable = Hit.GetActor();

			// Actor가 가지고 있는 Tag가 Item이라면 아이템 획득
			if (Interactable->ActorHasTag(TEXT("Item")))
			{
				UE_LOG(Pro4, Log, TEXT("Get %s"), *Interactable->GetName());
				
				NecGameInstance->PlayerMenu->AddItemToInventory(Interactable, 1);
			}
			else if (Interactable->ActorHasTag(TEXT("Door"))) // Actor가 가지고 있는 Tag가 문이면 문을 열기
			{
				ADoor* Door = Cast<ADoor>(Interactable);
				if (Door->GetCanPlayerOpenDoor())
				{
					Door->SetIsPlayerOpen(true);

					UE_LOG(Pro4, Warning, TEXT("Door"), *Interactable->GetName());
				}
			}
		}
	}
}

// UI 스위칭 함수
void APro4Character::ChangePlayerWidget()
{
	if (!NecGameInstance)
	{
		return;
	}
	UE_LOG(Pro4, Warning, TEXT("Change PlayerWidget."));

	NecGameInstance->PlayerMenu->ChangePlayerWidget();

}

void APro4Character::Server_DestroyItem_Implementation(AActor* DestroyActor)
{
	DestroyActor->Destroy();
}

/// <summary>
////////////////////////////////////////////////////// 잠식 상호작용 코드 ////////////////////////////////////////////////////////////
/// </summary>

// 잠식지역에 들어가고 벗어날 때 실행되는 함수
void APro4Character::NotifyActorBeginOverlap(AActor* Act)
{
	// Tag가 Encroach인 필드에 입장하면 잠식상태 함수 콜백
	if (Act->ActorHasTag(TEXT("Encroach")))
	{
		Encroached();
	}
}

void APro4Character::NotifyActorEndOverlap(AActor* Act)
{
	// Tag가 Encroach인 필드에 벗어나면 잠식상태 해제 함수 콜백
	if (Act->ActorHasTag(TEXT("Encroach")))
	{
		UnEncroached();
	}
}

#pragma region PlayerUI_Inventory_Section

/* 플레이어가 무기를 획득할 경우 실행되는 함수 */
void APro4Character::SetPlayerWeapon(AAWeapon* SetWeapon)
{
	// 플레이어가 습득한 무기 정보를 서버에 전달
	if (SetWeapon->GetItemName() == "AR" || SetWeapon->GetItemName() == "SR")
	{
		if (MainWeapon.bHaveWeapon)
		{
			SpawnWeaponItemOnServer(GetActorLocation(), MainWeapon.Weapon, MainWeapon.Scope, MainWeapon.Name, MainWeapon.IconPath, MainWeapon.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}
	else if (SetWeapon->GetItemName() == "Pistol")
	{
		if (SubWeapon.bHaveWeapon)
		{
			SpawnWeaponItemOnServer(GetActorLocation(), SubWeapon.Weapon, nullptr, SubWeapon.Name, SubWeapon.IconPath, SubWeapon.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}
	else // Knife
	{
		if (Knife.bHaveWeapon)
		{
			SpawnWeaponItemOnServer(GetActorLocation(), Knife.Weapon, nullptr, Knife.Name, Knife.IconPath, Knife.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}

	Server_DestroyItem(SetWeapon);
}

/* 클라이언트가 서버에게 드랍된 아이템의 상태를 설정하라고 알리는 함수. */
void APro4Character::SpawnWeaponItemOnServer_Implementation(FVector Location, USkeletalMesh* WeaponMesh, UStaticMesh* ScopeMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetPlayerController();
	FRotator Rotation;

	AAWeapon* DropItem = World->SpawnActor<AAWeapon>(AAWeapon::StaticClass(), Location, Rotation, SpawnParams);

	SpawnWeaponItemOnClient(DropItem, WeaponMesh, ScopeMesh, WeaponName, IconPath, ImagePath);
}

bool APro4Character::SpawnWeaponItemOnServer_Validate(FVector Location, USkeletalMesh* WeaponMesh, UStaticMesh* ScopeMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	return true;
}

/* NetMulticast로 호출됨. 서버가 클라이언트들에게 드랍된 무기 아이템의 설정을 뿌리는 함수. */
void APro4Character::SpawnWeaponItemOnClient_Implementation(AAWeapon* SpawnWeapon, USkeletalMesh* WeaponMesh, UStaticMesh* ScopeMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	SpawnWeapon->SetSKWeaponItem(WeaponMesh, ScopeMesh);
	SpawnWeapon->SetItemName(WeaponName);
	SpawnWeapon->SetIconPath(IconPath);
	SpawnWeapon->SetBoxImagePath(ImagePath);
	SpawnWeapon->SetItemNum(1);
}

/* 클라이언트가 서버에게 플레이어의 무기를 세팅하라고 알리는 함수 */
void APro4Character::NoticePlayerWeaponOnServer_Implementation(AAWeapon* _Weapon)
{
	NoticePlayerWeaponOnClient(_Weapon);
}

/* NetMulticast로 호출됨. 서버가 클라이언트들에게 해당 플레이어의 무기 설정을 뿌리는 함수. */
void APro4Character::NoticePlayerWeaponOnClient_Implementation(AAWeapon* _Weapon)
{
	Weapon->SetSkeletalMesh(_Weapon->GetSKWeaponItem());
	// 무기 타입에 따라 해당하는 변수에 아이템 정보 저장 
	if (_Weapon->GetItemName() == "AR" || _Weapon->GetItemName() == "SR")
	{
		MainWeapon.Weapon = _Weapon->GetSKWeaponItem();
		MainWeapon.Scope = _Weapon->GetSKScopeItem();
		MainWeapon.Name = _Weapon->GetItemName();
		MainWeapon.IconPath = _Weapon->GetIconPath();
		MainWeapon.ImagePath = _Weapon->GetBoxImagePath();

		// 무기 보유상태로 변경
		if (!MainWeapon.bHaveWeapon)
		{
			MainWeapon.bHaveWeapon = true;
		}

		// 스코프 소켓에 스코프 장착
		if (Weapon->DoesSocketExist("b_gun_scopeSocket"))
		{
			Scope->SetStaticMesh(_Weapon->GetSKScopeItem());
			Scope->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetIncludingScale, "b_gun_scopeSocket");
		}
	}
	else if (_Weapon->GetItemName() == "Pistol")
	{
		SubWeapon.Weapon = _Weapon->GetSKWeaponItem();
		SubWeapon.Name = _Weapon->GetItemName();
		SubWeapon.IconPath = _Weapon->GetIconPath();
		SubWeapon.ImagePath = _Weapon->GetBoxImagePath();
		
		// 무기 보유상태로 변경
		if (!SubWeapon.bHaveWeapon)
		{
			SubWeapon.bHaveWeapon = true;
		}
	}
	else
	{
		Knife.Weapon = _Weapon->GetSKWeaponItem();
		Knife.Name = _Weapon->GetItemName();
		Knife.IconPath = _Weapon->GetIconPath();
		Knife.ImagePath = _Weapon->GetBoxImagePath();

		// 무기 보유상태로 변경
		if (!Knife.bHaveWeapon)
		{
			Knife.bHaveWeapon = true;
		}
	}
}

/* 플레이어의 방어구를 착용하는 함수 */
void APro4Character::SetPlayerArmor(AAArmor* Armor)
{
	// 착용할 방어구가 헬멧일 경우
	if(Armor->GetItemName() == "Helmet")
	{ 
		// 플레이어가 이미 헬멧을 쓰고 있다면
		if (PlayerHelmet.bHaveArmor)
		{
			// 방어구 아이템 스폰을 위해 서버에게 알림.
			SpawnArmorItemOnServer(GetActorLocation(), PlayerHelmet.ArmorMesh, PlayerHelmet.ArmorName, PlayerHelmet.AP);
		}

		// 헬멧 아이템을 착용하기 위해 서버에게 알림.
		NoticePlayerArmorOnServer(Armor, Armor->GetItemName());
	}
	else
	{
		// 플레이어가 이미 방탄복을 입고 있다면
		if (PlayerVest.bHaveArmor)
		{
			// Spawn Drop Armor Item	
			SpawnArmorItemOnServer(GetActorLocation(), PlayerVest.ArmorMesh, PlayerVest.ArmorName, PlayerVest.AP);
		}

		NoticePlayerArmorOnServer(Armor, Armor->GetItemName());
	}

	Server_DestroyItem(Armor);
}

/* Client가 드랍한 방어구 아이템을 스폰해달라고 서버에게 알리는 함수 */
void APro4Character::SpawnArmorItemOnServer_Implementation(FVector Location, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.Owner = GetPlayerController();
		FRotator Rotation;

		AAArmor* DropItem = World->SpawnActor<AAArmor>(AAArmor::StaticClass(), Location, Rotation, SpawnParams);

		// 서버에서 아이템을 스폰하고, 해당 아이템의 정보를 Client들에게 뿌려줌
		SpawnArmorItemOnClient(DropItem, ArmorMesh, ArmorName, _AP);
	}
}

bool APro4Character::SpawnArmorItemOnServer_Validate(FVector Location, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP)
{
	return true;
}

/* NetMulticast로 실행되는 함수, 서버가 클라이언트들에게 생성된 아이템의 정보를 뿌려줌. */
void APro4Character::SpawnArmorItemOnClient_Implementation(AAArmor* SpawnArmor, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP)
{
	SpawnArmor->SetSKItem(ArmorMesh);
	SpawnArmor->SetItemName(ArmorName);
	SpawnArmor->SetCurrentAP(_AP);
	SpawnArmor->SetItemNum(1);
}

bool APro4Character::SpawnArmorItemOnClient_Validate(AAArmor* SpawnArmor, USkeletalMesh* ArmorMesh, const FString& ArmorName, float _AP)
{
	return true;
}

/* 서버에게 플레이어가 방어구를 착용한다고 알리는 함수 */
void APro4Character::NoticePlayerArmorOnServer_Implementation(AAArmor* _Armor, const FString& ArmorType)
{
	NoticePlayerArmorOnClient(_Armor, ArmorType);
}

/* NetMulticast로 실행되는 함수, 서버가 클라이언트들에게 업데이트 된 방어구 정보를 뿌려줌. */
void APro4Character::NoticePlayerArmorOnClient_Implementation(AAArmor* _Armor, const FString& ArmorType)
{
	// 방어구 유형에 따라 정보 저장
	if (ArmorType == "Helmet")
	{
		PlayerHelmet.ArmorName = _Armor->GetItemName();
		PlayerHelmet.ArmorMesh = _Armor->GetSKItem();
		PlayerHelmet.AP = _Armor->GetCurrentAP();

		Helmet->SetSkeletalMesh(PlayerHelmet.ArmorMesh);

		// 헬멧 장착 상태로 변경
		if (!PlayerHelmet.bHaveArmor)
		{
			PlayerHelmet.bHaveArmor = true;
		}
	}
	else
	{
		PlayerVest.ArmorName = _Armor->GetItemName();
		PlayerVest.ArmorMesh = _Armor->GetSKItem();
		PlayerVest.AP = _Armor->GetCurrentAP();

		Vest->SetSkeletalMesh(PlayerVest.ArmorMesh);

		// 방탄조끼 장착 상태로 변경
		if(!PlayerVest.bHaveArmor)
		{
			PlayerVest.bHaveArmor = true;
		}
	}

	CurrentAP = PlayerHelmet.AP + PlayerVest.AP;
}

// 투척무기 습득시 실행되는 함수
void APro4Character::AddPlayerGrenade(AAGrenade* _Grenade)
{
	if (NecGameInstance == nullptr)
	{
		return;
	}

	// 투척무기 유형에 따라서 해당하는 변수 값 증가
	if (!_Grenade->GetItemName().Compare("Grenade"))
	{
		PlayerGrenade.GrenadeNum++;

		NecGameInstance->PlayerMenu->AddItemToGrenade(_Grenade->GetItemName(), PlayerGrenade.GrenadeNum);
	}
	else if(!_Grenade->GetItemName().Compare("Smoke"))
	{
		PlayerGrenade.SmokeNum++;

		NecGameInstance->PlayerMenu->AddItemToGrenade(_Grenade->GetItemName(), PlayerGrenade.SmokeNum);
	}
	else if (!_Grenade->GetItemName().Compare("Flash"))
	{
		PlayerGrenade.FlashNum++;

		NecGameInstance->PlayerMenu->AddItemToGrenade(_Grenade->GetItemName(), PlayerGrenade.FlashNum);
	}

	Server_DestroyItem(_Grenade);
}

/* 탄약을 획득했을 때 실행되는 함수 */
void APro4Character::SetPlayerRound(AAmmo* _Ammo)
{
	if (_Ammo->GetItemName() == "MainWeaponAmmo")
	{
		MainWeapon.TotalRound += _Ammo->GetItemNum();
	}
	else
	{
		SubWeapon.TotalRound += _Ammo->GetItemNum();
	}

	Server_DestroyItem(_Ammo);
}
#pragma endregion

/* 플레이어 앞에있는 물건 확인하는 함수 */
void APro4Character::CheckFrontActorUsingTrace()
{
	FVector CharacterLoc;
	FRotator CharacterRot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(CharacterLoc, CharacterRot);

	FVector Start = CharacterLoc;
	FVector End = CharacterLoc + (CharacterRot.Vector() * 500);

	FCollisionQueryParams TraceParams;
	UWorld* World = GetWorld();

	bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// 커서에 닿은 물체가 있을때
	if (bHit)
	{
		// 닿은 물체의 클래스가 액터이면
		if (Hit.GetActor())
		{
			DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Red, false, 2.0f);
			DrawDebugString(GetWorld(), Hit.ImpactPoint - Start, TEXT("There are Something exist."), this, FColor::Green, 1.0f);

			AActor* HitActor = Hit.GetActor();

			// 아이템 태그 확인
			if (HitActor->ActorHasTag(TEXT("Item")))
			{
				AABaseItem* BaseItem = Cast<AABaseItem>(HitActor);
				switch (BaseItem->ItemType)
				{
				case AABaseItem::BaseItemType::Weapon:
				{
					AAWeapon* Hit_Weapon = Cast<AAWeapon>(BaseItem);
					Hit_Weapon->ViewWeaponName();
				}
					break;
				case AABaseItem::BaseItemType::Grenade:
				{
				
				}
					break;
				case AABaseItem::BaseItemType::Armor:
				{

				}
					break;
				case AABaseItem::BaseItemType::Ammo:
				{

				}
					break;
				case AABaseItem::BaseItemType::Recovery:
				{

				}
					break;
				case AABaseItem::BaseItemType::Vaccine:
				{

				}
					break;
				default:
					UE_LOG(Pro4, Error, TEXT("Player Trace Error"));
					break;
				}
			}
		}
	}
	else
	{

	}
}

/* Player Controller Class */
APro4PlayerController* APro4Character::GetPlayerController()
{
	return PlayerController;
}

void APro4Character::SetPlayerController(APro4PlayerController* _PlayerController)
{
	PlayerController = _PlayerController;
}

#pragma region PlayerHealth

/* 플레이어의 체력이 회복되었음을 서버에 알리는 함수 */
void APro4Character::RecoverPlayerHealthOnServer_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Recovery Player HP On Server");
	CurrentHP += 10.0f;

	if (CurrentHP >= MaxHP)
	{
		CurrentHP = MaxHP;
		bIsPlayerGetAttacked = false;
		GetWorldTimerManager().ClearTimer(HealthRecoveryTimer);
	}
}

/* 체력이 회복되었음을 서버에 알리기 전에, 체크하는 함수 */
bool APro4Character::RecoverPlayerHealthOnServer_Validate()
{
	return true;
}

/* UFUNCTION(Client)로 실행, 해당 캐릭터를 조종하고 있는 클라이언트에게 죽었다는 메세지를 날려주는 함수 */
void APro4Character::PlayerDead_Implementation()
{
	AInGamePlayerState* ThisPlayerState = Cast<AInGamePlayerState>(GetPlayerState());
	APro4PlayerController* ThisPlayerController = Cast<APro4PlayerController>(GetController());

	NecGameInstance->PlayerMenu->ActiveGameOverUI(
		ThisPlayerState->GetPlayerKill(),
		ThisPlayerState->GetZombieKill(),
		ThisPlayerController->SetPlayerRankning(),
		ThisPlayerController->GetTotalRanking()
	);
	
	Server_DestroyItem(this);
}

// 플레이어 체력이 닳았을 때
void APro4Character::PlayerHealthGetDamagedOnServer_Implementation(float Damage, AActor* AttackActor)
{
	CurrentHP -= Damage;

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		AInGamePlayerState* ThisPlayerState = Cast<AInGamePlayerState>(GetPlayerState());

		ThisPlayerState->SetIsDead(true);

		RecordPlayerKill(AttackActor);
		// Player Dead
		PlayerDead();
	}

	/* 플레이어가 공격받지 않는 상황에서 맞았을 경우 */
	if (!bIsPlayerGetAttacked)
	{
		// 회복을 하기위해 타이머를 설정, 현재 플레이어의 상태 : 피격상태
		bIsPlayerGetAttacked = true;
	}

	GetWorldTimerManager().ClearTimer(HealthRecoveryTimer);
	GetWorldTimerManager().SetTimer(HealthRecoveryTimer, this, &APro4Character::RecoverPlayerHealthOnServer, 1.0f, true, 5.0f);
}

bool APro4Character::PlayerHealthGetDamagedOnServer_Validate(float Damage, AActor* AttackActor)
{

	if (Damage >= 100.0f || Damage < 0.0f)
	{
		return false;
	}
	
	return true;
}

// 플레이어 피격시
void APro4Character::GetDamaged(float Damage, AActor* AttackActor)
{
	if (GetWorld()->IsServer())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Player Get Damaged"));
		PlayerHealthGetDamagedOnServer(Damage, AttackActor);
	}
}

void APro4Character::RecordPlayerKill_Implementation(AActor* AttackActor)
{
	if (AttackActor->ActorHasTag("Player"))
	{
		// 다른 플레이어에게 죽은 경우, 다른 플레이어의 킬수를 올려줌.
		APro4Character* OtherPlayer = Cast<APro4Character>(AttackActor);
		AInGamePlayerState* AttackPlayerState = Cast<AInGamePlayerState>(OtherPlayer->GetPlayerState());

		// 다른 플레이어의 플레이어 스테이트가 없다면!
		// 이 함수는 서버에서 실행되기 때문의 모든 플레이어의 스테이트를 보존하고 있음!
		if (!AttackPlayerState)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("There are no Playerstate in Other Player Pawn."));
		}
		else
		{
			FString TargetType = "Player";
			AttackPlayerState->UpdatePlayerKillInfo(TargetType, AttackActor);
		}

	}
	else
	{
		// 좀비한테 죽은 경우
		;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Record Player Kill"));
}

#pragma endregion

#pragma region ZombieSpawner

void APro4Character::ZombieSpawnerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("ZombieSpawner"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("ZombieSpawner is Detected."));

		AZombieSpawner* ZombieSpawner = Cast<AZombieSpawner>(OtherActor);
		if (SpawnZombieCurCount < SpawnZombieMaxCount)
		{
			ZombieSpawner->PlayerOverlapToZSpawner(GetInstigator());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("20 zombies have already been spawned."));
		}
	}


	/* Draw Player's ZombieSpawner Detection Extent */
	DrawDebugBox(GetWorld(), GetActorLocation(), DetectExtent, FColor::Green, false, 5.0f, 0, 10.0f);
}

void APro4Character::ZombieSpawnerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("ZombieSpawner"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, TEXT("ZombieSpawner is Detected."));

		AZombieSpawner* ZombieSpawner = Cast<AZombieSpawner>(OtherActor);
		ZombieSpawner->PlayerAwayFromSpawner(GetInstigator());
	}

	/* Draw Player's ZombieSpawner Detection Extent */
	DrawDebugBox(GetWorld(), GetActorLocation(), DetectExtent, FColor::Red, false, 5.0f, 0, 10.0f);
}


void APro4Character::DetectZombieSpawner(bool isNight)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Detect Zombie Spawner"));
	DetectZSpawnerCol->SetGenerateOverlapEvents(isNight);
}

#pragma endregion

#pragma region EquipPlayerWeapon
void APro4Character::EquipGrenade()
{
	CurrentWeaponMode = WeaponMode::ATW;

	if (PlayerGrenade.EquipGrenade == "Grenade")	// 현재 들고 있는 투척무기가 수류탄인 경우
	{
		if (PlayerGrenade.SmokeNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Smoke";
		}
		else if (PlayerGrenade.FlashNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Flash";
		}
		else
		{
			CurrentWeaponMode = WeaponMode::Disarming;
			PlayerGrenade.EquipGrenade = "None";
		}

		return;
	}
	else if(PlayerGrenade.EquipGrenade == "Smoke")	// 현재 들고 있는 투척무기가 연막탄인 경우
	{
		if (PlayerGrenade.FlashNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Flash";
		}
		else
		{
			CurrentWeaponMode = WeaponMode::Disarming;
			PlayerGrenade.EquipGrenade = "None";
		}
	}
	else if (PlayerGrenade.EquipGrenade == "Flash")	// 현재 들고 있는 투척무기가 섬광탄인 경우
	{
		CurrentWeaponMode = WeaponMode::Disarming;
		PlayerGrenade.EquipGrenade = "None";
	}
	else // 현재 아무것도 들고 있지 않은 경우
	{
		if (PlayerGrenade.GrenadeNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Grenade";
		}
		else if (PlayerGrenade.SmokeNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Smoke";
		}
		else if (PlayerGrenade.FlashNum > 0)
		{
			PlayerGrenade.EquipGrenade = "Flash";
		}
		else
		{
			CurrentWeaponMode = WeaponMode::Disarming;
			PlayerGrenade.EquipGrenade = "None";
		}
	}
}

void APro4Character::EquipPlayerWeaponOnServer_Implementation(const WeaponMode& _CurWeaponMode, UStaticMesh* GrenadeMesh = nullptr)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("EquipPlayerWeaponOnServer"));
	EquipPlayerWeaponOnClient(_CurWeaponMode, GrenadeMesh);
}

void APro4Character::EquipPlayerWeaponOnClient_Implementation(const WeaponMode& _CurWeaponMode, UStaticMesh* GrenadeMesh = nullptr)
{
	switch (_CurWeaponMode)
	{
	case WeaponMode::Main:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("MainWeapon"));
		Weapon->SetSkeletalMesh(MainWeapon.Weapon);
		MuzzleFlash->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "gunFireLocation");
		NecGameInstance->PlayerMenu->ActiveWeaponShortcut(1);
		break;
	case WeaponMode::Sub:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("SubWeapon"));
		Weapon->SetSkeletalMesh(SubWeapon.Weapon);
		MuzzleFlash->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "gunFireLocation");
		NecGameInstance->PlayerMenu->ActiveWeaponShortcut(2);
		break;
	case WeaponMode::Knife:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Knife"));
		Weapon->SetSkeletalMesh(Knife.Weapon);
		MuzzleFlash->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "gunFireLocation");
		NecGameInstance->PlayerMenu->ActiveWeaponShortcut(3);
		break;
	case WeaponMode::ATW:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("ATW"));
		Grenade->SetStaticMesh(GrenadeMesh);
		
		NecGameInstance->PlayerMenu->ActiveWeaponShortcut(4);
		break;
	case WeaponMode::Disarming:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Disarming"));
		Weapon->SetSkeletalMesh(nullptr);
		Grenade->SetStaticMesh(nullptr);
		Scope->SetStaticMesh(nullptr);
		PlayerGrenade.EquipGrenade = "None";
		NecGameInstance->PlayerMenu->ActiveWeaponShortcut(0);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("_CurWeaponMode Variable has garbage value."));
		break;
	}
}

#pragma endregion

#pragma region Escape

void APro4Character::SetIsPossibleEscapeOnServer_Implementation(bool Escape)
{
	IsPossibleEscape = Escape;
}

/* 백신을 가졌을 때, 탈출하기 위한 헬리콥터를 부르는 함수 */
void APro4Character::CallHelicopterToEscapeOnServer_Implementation()
{
	if (!BP_Helicopter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CANT FIND BP_Helicopter"));
		return;
	}

	if (!GetIsPossibleEscape())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("This Player can't Escape."));
		return;
	}

	APro4PlayerController* NecrophobiaPlayerController = Cast<APro4PlayerController>(GetController());

	if (!NecrophobiaPlayerController->SetHelicopterSpawn())
	{
		// 헬리콥터를 부를 수 없을 때 실행되는 함수
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Can't spawn helicopter."));
		return;
	}

	DrawDebugSolidBox(GetWorld(), GetActorLocation(), FVector(20.0f), FColor::Blue, true);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// 헬리콥터가 생성될 좌표
	FVector SpawnLocation = FVector::ZeroVector;
	SpawnLocation.Z = 1000.0f;

	FVector ToPlayerVector = GetActorLocation() - SpawnLocation;
	ToPlayerVector.Z = 0.0f;
	ToPlayerVector.Normalize();

	FRotator SpawnRotation = ToPlayerVector.Rotation();

	AActor* SpawnHeliActor = GetWorld()->SpawnActor(BP_Helicopter->GeneratedClass);
	AHeli_AH64D* SpawnHelicopter = Cast<AHeli_AH64D>(SpawnHeliActor);

	if (SpawnHelicopter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Helicopter Spawn!"));
	}

	SpawnHelicopter->SetHelicopterSetting(GetActorLocation(), SpawnLocation, SpawnRotation);
}

/* 플레이어가 탈출에 성공했을 때 실행되는 함수 */
void APro4Character::PlayerEscape()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Player Escape"));

	APro4PlayerController* NecrophobiaPlayerController = Cast<APro4PlayerController>(GetController());

	NecrophobiaPlayerController->AvaialbleHelicopterSpawnOnServer();

}
#pragma endregion

#pragma region EncroachField

/* 잠식 치료제를 사용했을 때, 플레이어의 잠식도를 치료하는 함수 */
void APro4Character::RecoveryEncroach_Implementation()
{
	if (EncroachLevel > 0)
	{
		EncroachLevel--;
	}
	else
	{
		return;
	}

	if (MaxHP >= 50 && MaxHP < 100)
	{
		MaxHP = 100 - 10 * EncroachLevel;

		GetWorldTimerManager().ClearTimer(HealthRecoveryTimer);
		GetWorldTimerManager().SetTimer(HealthRecoveryTimer, this, &APro4Character::RecoverPlayerHealthOnServer, 1.0f, true, 5.0f);
	}
}

void APro4Character::StartEncroachTimer()
{
	if (IsEncroach && GetWorld()->IsServer())
	{
		GetWorldTimerManager().SetTimer(EncroachTimer, this, &APro4Character::SetPlayerEncroach, 5.0f, true);
	}
}

void APro4Character::SetPlayerEncroach_Implementation() 
{
	if (EncroachLevel < 5)
	{
		EncroachLevel++;
	}
	
	MaxHP = 100 - 10 * EncroachLevel;

	if (CurrentHP >= MaxHP)
	{
		CurrentHP = MaxHP;
	}
}

void APro4Character::StopEncroachTimer()
{
	if (!IsEncroach && GetWorld()->IsServer())
	{
		GetWorldTimerManager().ClearTimer(EncroachTimer);
	}
}
#pragma endregion

void APro4Character::PlayMontageOnServer_Implementation(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0)
{
	PlayMontageOnClient(AnimationMontage, SectionNumber);
}

void APro4Character::PlayMontageOnClient_Implementation(UAnimMontage* AnimationMontage, uint16 SectionNumber = 0)
{
	Pro4Anim->Montage_Play(AnimationMontage, 1.0f);

	if (SectionNumber)
	{
		FName Section(FString::FromInt(SectionNumber));
		Pro4Anim->Montage_JumpToSection(Section, AnimationMontage);
	}
}

void APro4Character::SetPlayerStateOnServer_Implementation(const FString& State, bool bIsState)
{
	if (State == "Run")
	{
		IsRun = bIsState;
	}
	else if (State == "Zoom")
	{
		IsZoom = bIsState;
	}
}

void APro4Character::SetPlayerFlagOnServer_Implementation(const FString& State, int32 Flag)
{
	if (State == "EquipFlag")
	{
		Equipflag = Flag;
	}
	else if (State == "MoveFlag")
	{
		Moveflag = Flag;
	}
}

void APro4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APro4Character, CurrentHP);
	DOREPLIFETIME(APro4Character, MaxHP);
	DOREPLIFETIME(APro4Character, CurrentAP);
	DOREPLIFETIME(APro4Character, bIsPlayerGetAttacked);
	DOREPLIFETIME(APro4Character, bIsRecoveryTimerStarted);
	DOREPLIFETIME(APro4Character, IsRun);
	DOREPLIFETIME(APro4Character, IsZoom);
	DOREPLIFETIME(APro4Character, Equipflag);
	DOREPLIFETIME(APro4Character, Moveflag);
	DOREPLIFETIME(APro4Character, IsDead);
}