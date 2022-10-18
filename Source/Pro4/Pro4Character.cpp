// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Character.h"
#include "Pro4AnimInstance.h"
#include "NecrophobiaGameInstance.h"
#include "UserInterface/PlayerMenu.h"
#include "Item/AWeapon.h"
#include "Item/AArmor.h"
#include "Item/AGrenade.h"
#include "Door.h"

#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APro4Character::APro4Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HELMET"));
	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VEST"));

	MapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MAPSPRINGARM"));
	MapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MAPCAPTURE"));

	RootComponent = GetCapsuleComponent();

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	MapSpringArm->SetupAttachment(GetCapsuleComponent());
	MapCapture->SetupAttachment(MapSpringArm);
	MapSpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PCharacter"));

	CameraSetting();
	MovementSetting();
	WeaponSetting();
	StateSetting();

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

	SocketSetting();

	Tags.Add("Player");
}

// Called when the game starts or when spawned
void APro4Character::BeginPlay()
{
	Super::BeginPlay();
	FString IsServer = "False";
	if (GetWorld()->IsServer())
	{
		IsServer = "True";
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 150), (TEXT("Is server? : %s"), *IsServer), this, FColor::Red, 15.0f);
}

/// <summary>
////////////////////////////////////////////////////// 초기세팅 ////////////////////////////////////////////////////////////
/// </summary>

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

	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 50.0f);

	MapSpringArm->bInheritPitch = true;
	MapSpringArm->bInheritRoll = true;
	MapSpringArm->bInheritYaw = true;

	MapCapture->ProjectionType = ECameraProjectionMode::Perspective;
	MapCapture->OrthoWidth = 1000.0f;
}

void APro4Character::MovementSetting()
{
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	CurrentCharacterState = CharacterState::Standing;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	IsRun = false;
	IsHold = false;
	IsZoom = false;
	IsForward = true;
	Moveflag = 0;
	Updownflag=0;
	LeftRightflag=0;
}

void APro4Character::WeaponSetting()
{
	ProjectileClass = APro4Projectile::StaticClass();
	Equipflag = 0;
	IsEquipping = false;
	IsReloading = false;
	FireMod = false;
}


void APro4Character::StateSetting()
{
	MaxHP = 100.0f;
	CurrentHP = 100.0f;
	MaxAP = 100.0f;
	CurrentAP = 20.0f;

	HoldTime = 0.0f;
	HoldFlag = 0;

	EncroachLevel = 0;
	IsEncroach = false;
	EncroachTime = 0.0f;
}

void APro4Character::SocketSetting()
{
	FName WeaponSocket(TEXT("Hand_rSocket"));
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
}

/// <summary>
////////////////////////////////////////////////////// 초기세팅 ////////////////////////////////////////////////////////////
/// </summary>

// Called every frame
void APro4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsHold)
	{
		HoldTime += DeltaTime;
		switch (HoldFlag)
		{
		case 1:
			if (HoldTime >= 0.3f)
			{
				IsHold = false;
				HoldTime = 0.0f;
				HoldFlag = 0;
			}
			break;
		case 2:
			if (HoldTime >= 1.2f)
			{
				IsHold = false;
				HoldTime = 0.0f;
				HoldFlag = 0;
			}
			break;
		}
	}

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

	/* Player HealthPoint Section */
	/*CurrentHP += 1.0f;
	if (CurrentHP >= 90.0f) 
	{
		CurrentHP = 10.0f;
	}*/

	if (IsEncroach)
	{
		EncroachTime += DeltaTime;
		if (EncroachTime > 5.0f)
		{
			EncroachLevel++;
			switch (EncroachLevel)
			{
			case 1:
				MaxHP = 90;
				if (CurrentHP > 90)
				{
					CurrentHP = 90;
				}
				break;
			case 2:
				MaxHP = 80;
				if (CurrentHP > 80)
				{
					CurrentHP = 80;
				}
				break;
			default:
				break;
			}
			EncroachTime = 0.0f;
		}
	}

	/* Trace하는 함수 */
	// CheckFrontActorUsingTrace();
	

	// Character Role Test.
	DrawDebugString(GetWorld(), FVector(0, 0, 150), GetEnumRole(GetLocalRole()), this, FColor::Green, DeltaTime);
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

void APro4Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Pro4Anim = Cast<UPro4AnimInstance>(GetMesh()->GetAnimInstance());

	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnEquipMontageEnded);
	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnReloadMontageEnded);
	Pro4Anim->OnMontageEnded.AddDynamic(this, &APro4Character::OnAttackMontageEnded);
}

void APro4Character::OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsEquipping = false;
}

void APro4Character::OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsMontagePlay = false;
	IsReloading = false;
}

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
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this, &APro4Character::Prone);
	PlayerInputComponent->BindAction(TEXT("Key1"), EInputEvent::IE_Pressed, this, &APro4Character::EquipMain1);
	PlayerInputComponent->BindAction(TEXT("Key2"), EInputEvent::IE_Pressed, this, &APro4Character::EquipMain2);
	PlayerInputComponent->BindAction(TEXT("Key3"), EInputEvent::IE_Pressed, this, &APro4Character::EquipSub);
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

float APro4Character::UpdownSpeed()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Standing:
		if (IsRun)
		{
			if (Updownflag == 1)
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
			if (Updownflag == 1)
			{
				return 0.5f;
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
	case CharacterState::Proning:
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

float APro4Character::LeftRightSpeed()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Standing:
		if (IsRun)
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
	case CharacterState::Proning:
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

void APro4Character::UpDown(float NewAxisValue)
{
	if (!IsHold)
	{
		if (NewAxisValue > 0.5f || NewAxisValue < -0.5f)
		{
			IsForward = true;
			if (NewAxisValue < 0)
			{
				Moveflag = 1;
				Updownflag = -1;
			}
			else
			{
				Moveflag = 0;
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

void APro4Character::LeftRight(float NewAxisValue)
{
	if (!IsHold)
	{
		if (NewAxisValue > 0.5f || NewAxisValue < -0.5f)
		{
			IsForward = false;
			if (NewAxisValue < 0)
			{
				Moveflag = 1;
				LeftRightflag = 1;
			}
			else
			{
				Moveflag = 0;
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

void APro4Character::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void APro4Character::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void APro4Character::Run()
{
	if(CurrentCharacterState==CharacterState::Standing)
	IsRun = !IsRun;
}

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
		case CharacterState::Proning:
			HoldFlag = 2;
			UE_LOG(Pro4, Log, TEXT("Stand."));
			CurrentCharacterState = CharacterState::Standing;
			break;
		}
	}

}

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
			case CharacterState::Proning:
				Super::Crouch();
				HoldFlag = 1;
				CurrentCharacterState = CharacterState::Crouching;
				break;
			}
		}
	}
}

void APro4Character::Prone()
{
	if (!IsHold)
	{
		IsHold = true;
		switch (CurrentCharacterState)
		{
		case CharacterState::Standing:
			UE_LOG(Pro4, Log, TEXT("Prone."));
			HoldFlag = 2;
			CurrentCharacterState = CharacterState::Proning;
			break;
		case CharacterState::Crouching:
			Super::UnCrouch();
			HoldFlag = 2;
			CurrentCharacterState = CharacterState::Proning;
			break;
		case CharacterState::Proning:
			UE_LOG(Pro4, Log, TEXT("Stand."));
			HoldFlag = 2;
			CurrentCharacterState = CharacterState::Standing;
			break;
		}
	}
}
/// <summary>
////////////////////////////////////////////////////// 캐릭터 움직임 코드 ////////////////////////////////////////////////////////////
/// </summary>

/// <summary>
////////////////////////////////////////////////////// 캐릭터 무기장착, 장전 코드 ////////////////////////////////////////////////////////////
/// </summary>

void APro4Character::EquipMain1()
{
	if (CurrentWeaponMode == WeaponMode::Main1)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		Equipflag = 0;
		CurrentWeaponMode = WeaponMode::Disarming;
		Weapon->SetSkeletalMesh(nullptr);
	}
	else
	{
		if (IsEquipping) return;
		Equipflag = 1;
		Pro4Anim->PlayEquipMontage();
		Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->EquipMontage);
		IsMontagePlay = true;
		IsEquipping = true;
		CurrentWeaponMode = WeaponMode::Main1;
		Weapon->SetSkeletalMesh(MainWeapon.Weapon);
	}
}

void APro4Character::EquipMain2()
{
	if (CurrentWeaponMode == WeaponMode::Main2)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		Equipflag = 0;
		CurrentWeaponMode = WeaponMode::Disarming;
		Weapon->SetSkeletalMesh(nullptr);
	}
	else
	{
		if (IsEquipping) return;
		UE_LOG(Pro4, Log, TEXT("EquipMain2."));
		Equipflag = 1;
		Pro4Anim->PlayEquipMontage();
		Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->EquipMontage);
		IsMontagePlay = true;
		IsEquipping = true;
		CurrentWeaponMode = WeaponMode::Main2;
		Weapon->SetSkeletalMesh(SubWeapon.Weapon);
	}
}

void APro4Character::EquipSub()
{
	if (CurrentWeaponMode == WeaponMode::Sub)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		CurrentWeaponMode = WeaponMode::Disarming;
		Weapon->SetSkeletalMesh(nullptr);
	}
	else
	{
		if (IsEquipping) return;
		UE_LOG(Pro4, Log, TEXT("EquipSub."));
		Equipflag = 2;
		Pro4Anim->PlayEquipMontage();
		Pro4Anim->Montage_JumpToSection(FName("2"), Pro4Anim->EquipMontage);
		IsMontagePlay = true;
		IsEquipping = true;
		CurrentWeaponMode = WeaponMode::Sub;
		Weapon->SetSkeletalMesh(Knife.Weapon);
	}
}

void APro4Character::EquipATW()
{
	if (CurrentWeaponMode == WeaponMode::ATW)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		CurrentWeaponMode = WeaponMode::Disarming;
	}
	else
	{
		UE_LOG(Pro4, Log, TEXT("ATW."));
		CurrentWeaponMode = WeaponMode::ATW;
	}
}

void APro4Character::Reload()
{
	if (IsReloading)
		return;
	if (!IsMontagePlay)
	{
		switch (CurrentWeaponMode)
		{
		case WeaponMode::Main1:
			if (CurrentCharacterState == CharacterState::Standing)
			{
				Pro4Anim->PlayReloadMontage();
				Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->ReloadMontage);
				IsMontagePlay = true;
				IsReloading = true;
			}
			else if (CurrentCharacterState == CharacterState::Crouching)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			else if (CurrentCharacterState == CharacterState::Proning)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			break;
		case WeaponMode::Main2:
			if (CurrentCharacterState == CharacterState::Standing)
			{
				Pro4Anim->PlayReloadMontage();
				Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->ReloadMontage);
				IsMontagePlay = true;
				IsReloading = true;
			}
			else if (CurrentCharacterState == CharacterState::Crouching)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			else if (CurrentCharacterState == CharacterState::Proning)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			break;
		case WeaponMode::Sub:
			if (CurrentCharacterState == CharacterState::Standing)
			{
				Pro4Anim->PlayReloadMontage();
				Pro4Anim->Montage_JumpToSection(FName("2"), Pro4Anim->ReloadMontage);
				IsMontagePlay = true;
				IsReloading = true;
			}
			else if (CurrentCharacterState == CharacterState::Crouching)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			else if (CurrentCharacterState == CharacterState::Proning)
			{
				UE_LOG(Pro4, Log, TEXT("Reload."));
			}
			break;
		case WeaponMode::ATW:
			UE_LOG(Pro4, Log, TEXT("Reload."));
			break;
		case WeaponMode::Disarming:
			UE_LOG(Pro4, Log, TEXT("Reload."));
			break;
		}
	}
}
/// <summary>
////////////////////////////////////////////////////// 캐릭터 무기장착, 장전 코드 ////////////////////////////////////////////////////////////
/// </summary>

#pragma region Character_Attack

void APro4Character::Attack()
{
	if (!IsEquipping)
	{
		switch (CurrentWeaponMode)
		{
		case WeaponMode::Main1:
			Fire();
			break;
		case WeaponMode::Main2:
			Fire();
			break;
		case WeaponMode::Sub:
			// if(���Ⱑ �����̸�)
			// Swing();
			// else(���Ⱑ �����̸�)
			// Fire();
			break;
		case WeaponMode::ATW:
			Throw();
			break;
		case WeaponMode::Disarming:
			Punch();
			break;
		}
	}
}

void APro4Character::Zoom() // �� ��, �� �ƿ�(�ѵ����������)
{
	if (CurrentWeaponMode == WeaponMode::Main1 || CurrentWeaponMode == WeaponMode::Main2 || CurrentWeaponMode == WeaponMode::Sub)
	{
		IsZoom = !IsZoom;
	}
}

void APro4Character::Fire_Mod() // ������, �ܹ߸�� ����
{
	if (CurrentWeaponMode == WeaponMode::Main1 || CurrentWeaponMode == WeaponMode::Main2)
		FireMod = !FireMod;
}

void APro4Character::StartFire() // ���콺 Ŭ���� �� �ݵǼ� ��� ���·�
{
	IsFire = true;
	Attack();
}

void APro4Character::StopFire() // ���콺 �� �� �ݵǼ� ��� ���� ���·�
{
	IsFire = false;
}

void APro4Character::Fire()
{
	if (IsFire)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation;
		if (Weapon != nullptr)
		{
			if (Weapon->DoesSocketExist("gunFireLocation"))
			{
				MuzzleLocation = Weapon->GetSocketLocation("gunFireLocation");
			}

		}
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		if (CurrentWeaponMode == WeaponMode::Main1 || CurrentWeaponMode == WeaponMode::Main2) // 주무기일 때의 총알 발사 (탄창 상태 반영 안함)
		{
			if (!IsMontagePlay)
			{
				if (IsZoom)
				{
					Pro4Anim->PlayAttackMontage();
					Pro4Anim->Montage_JumpToSection(FName("2"), Pro4Anim->AttackMontage);
					IsMontagePlay = true;
					IsAttacking = true;
					UE_LOG(Pro4, Log, TEXT("2"));
				}
				else
				{
					Pro4Anim->PlayAttackMontage();
					Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->AttackMontage);
					IsMontagePlay = true;
					IsAttacking = true;
					UE_LOG(Pro4, Log, TEXT("1"));
				}
			}

			SpawnProjectileOnServer(MuzzleLocation, MuzzleRotation, MuzzleRotation.Vector(), this);

			if (FireMod) // 연사 상태이면 함수 딜레이 후 다시 콜 (주무기 종류에 따라서 연사속도 변경)
			{
				GetWorld()->GetTimerManager().SetTimer(FireDelay, this, &APro4Character::Fire, .075f, false);
			}
		}
		else // 권총 발사 (권총용 애니메이션, 탄속, 권총 오브젝트 크기에 따라 총 발사 위치 조정해야 함)
		{
			UE_LOG(Pro4, Log, TEXT("Pistol Fire"));
		}
	}
}
	

void APro4Character::Throw() // 투척무기 던지기
{
	/*
	* 던지는 애니메이션
	*/
	UE_LOG(Pro4, Log, TEXT("ATW Throw"));
}

void APro4Character::Punch() // 주먹질
{
	/*
	* 주먹질 애니메이션 꾹 눌렀을 때 주먹질 계속하도록
	*/
	UE_LOG(Pro4, Log, TEXT("Punch"));
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
#pragma endregion



/// <summary>
/// F 버튼을 눌렀을 때 아이템, 문 등 상호작용을 하기위한 함수
/// </summary>
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

	if (bHit)
	{
		if (Hit.GetActor()) {
			DrawDebugSolidBox(World, Hit.ImpactPoint, FVector(5.0f), FColor::Blue, false, 2.0f);
			DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Red, false, 2.0f);

			UE_LOG(Pro4, Log, TEXT("HitActor : %s"), *Hit.GetActor()->GetName());

			AActor* Interactable = Hit.GetActor();

			// Actor가 가지고 있는 Tag가 Item이라면.
			if (Interactable->ActorHasTag(TEXT("Item")))
			{
				UNecrophobiaGameInstance* Instance = Cast<UNecrophobiaGameInstance>(GetGameInstance());
				UE_LOG(Pro4, Log, TEXT("Get %s"), *Interactable->GetName());
				
				Instance->PlayerMenu->AddItemToInventory(Interactable, 1);
			}
			else if (Interactable->ActorHasTag(TEXT("Door")))
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

void APro4Character::ChangePlayerWidget()
{
	UNecrophobiaGameInstance* Instance = Cast<UNecrophobiaGameInstance>(GetGameInstance());

	UE_LOG(Pro4, Warning, TEXT("Change PlayerWidget."));

	Instance->PlayerMenu->ChangePlayerWidget();

}

void APro4Character::Server_DestroyItem_Implementation(AActor* DestroyActor)
{
	DestroyActor->Destroy();
}

/// <summary>
////////////////////////////////////////////////////// 잠식 상호작용 코드 ////////////////////////////////////////////////////////////
/// </summary>

void APro4Character::NotifyActorBeginOverlap(AActor* Act)
{
	if (Act->ActorHasTag(TEXT("Encroach")))
	{
		Encroached();
	}
}

void APro4Character::NotifyActorEndOverlap(AActor* Act)
{
	if (Act->ActorHasTag(TEXT("Encroach")))
	{
		UnEncroached();
	}
}


#pragma region PlayerUI_Inventory_Section
/* 플레이어가 무기를 획득할 경우 실행되는 함수 */
void APro4Character::SetPlayerWeapon(AAWeapon* SetWeapon)
{
	if (SetWeapon->GetItemName() == "AR" || SetWeapon->GetItemName() == "SR")
	{
		if (MainWeapon.bHaveWeapon)
		{

			SpawnWeaponItemOnServer(GetActorLocation(), MainWeapon.Weapon, MainWeapon.Name, MainWeapon.IconPath, MainWeapon.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}
	else if (SetWeapon->GetItemName() == "Pistol")
	{
		if (SubWeapon.bHaveWeapon)
		{
			SpawnWeaponItemOnServer(GetActorLocation(), SubWeapon.Weapon, SubWeapon.Name, SubWeapon.IconPath, SubWeapon.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}
	else // Knife
	{
		if (Knife.bHaveWeapon)
		{
			SpawnWeaponItemOnServer(GetActorLocation(), Knife.Weapon, Knife.Name, Knife.IconPath, Knife.ImagePath);
		}

		NoticePlayerWeaponOnServer(SetWeapon);
	}

	Server_DestroyItem(SetWeapon);
}

/* 클라이언트가 서버에게 드랍된 아이템의 상태를 설정하라고 알리는 함수. */
void APro4Character::SpawnWeaponItemOnServer_Implementation(FVector Location, USkeletalMesh* WeaponMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetPlayerController();
	FRotator Rotation;

	AAWeapon* DropItem = World->SpawnActor<AAWeapon>(AAWeapon::StaticClass(), Location, Rotation, SpawnParams);

	SpawnWeaponItemOnClient(DropItem, WeaponMesh, WeaponName, IconPath, ImagePath);
}

bool APro4Character::SpawnWeaponItemOnServer_Validate(FVector Location, USkeletalMesh* WeaponMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	return true;
}

/* NetMulticast로 호출됨. 서버가 클라이언트들에게 드랍된 무기 아이템의 설정을 뿌리는 함수. */
void APro4Character::SpawnWeaponItemOnClient_Implementation(AAWeapon* SpawnWeapon, USkeletalMesh* WeaponMesh, const FString& WeaponName, const FString& IconPath, const FString& ImagePath)
{
	SpawnWeapon->SetSKWeaponItem(WeaponMesh);
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

	if (_Weapon->GetItemName() == "AR" || _Weapon->GetItemName() == "SR")
	{
		MainWeapon.Weapon = _Weapon->GetSKWeaponItem();
		MainWeapon.Name = _Weapon->GetItemName();
		MainWeapon.IconPath = _Weapon->GetIconPath();
		MainWeapon.ImagePath = _Weapon->GetBoxImagePath();

		if (!MainWeapon.bHaveWeapon)
		{
			MainWeapon.bHaveWeapon = true;
		}
	}
	else if (_Weapon->GetItemName() == "Pistol")
	{
		SubWeapon.Weapon = _Weapon->GetSKWeaponItem();
		SubWeapon.Name = _Weapon->GetItemName();
		SubWeapon.IconPath = _Weapon->GetIconPath();
		SubWeapon.ImagePath = _Weapon->GetBoxImagePath();
		
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
	if (ArmorType == "Helmet")
	{
		PlayerHelmet.ArmorName = _Armor->GetItemName();
		PlayerHelmet.ArmorMesh = _Armor->GetSKItem();
		PlayerHelmet.AP = _Armor->GetCurrentAP();

		Helmet->SetSkeletalMesh(PlayerHelmet.ArmorMesh);

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

		if(!PlayerVest.bHaveArmor)
		{
			PlayerVest.bHaveArmor = true;
		}
	}
}

void APro4Character::AddPlayerGrenade(AAGrenade* Grenade)
{
	Server_DestroyItem(Grenade);
}
#pragma endregion

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

	if (bHit)
	{
		if (Hit.GetActor())
		{
			DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Red, false, 2.0f);
			DrawDebugString(GetWorld(), Hit.ImpactPoint - Start, TEXT("There are Something exist."), this, FColor::Green, 1.0f);

			AActor* HitActor = Hit.GetActor();

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
				case AABaseItem::BaseItemType::Parts:
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

	if (CurrentHP >= 100)
	{
		bIsPlayerGetAttacked = false;
		GetWorldTimerManager().ClearTimer(HealthRecoveryTimer);
	}
}

bool APro4Character::RecoverPlayerHealthOnServer_Validate()
{
	return true;
}

void APro4Character::PlayerHealthGetDamagedOnServer_Implementation(float Damage)
{
	CurrentHP -= Damage;

	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		UE_LOG(Pro4, Warning, TEXT("Player is dead."));
	}

	/* 플레이어가 공격받지 않는 상황에서 맞았을 경우 */
	if (!bIsPlayerGetAttacked)
	{
		// 회복을 하기위해 타이머를 설정, 현재 플레이어의 상태 : 피격상태
		bIsPlayerGetAttacked = true;
		GetWorldTimerManager().SetTimer(HealthRecoveryTimer, this, &APro4Character::RecoverPlayerHealthOnServer, 1.0f, true, 5.0f);
	}
	else
	{
		// 다시 공격받았을 경우, 타이머를 리셋하고 다시 설정. 현재 플레이어의 상태 : 피격상태
		GetWorldTimerManager().ClearTimer(HealthRecoveryTimer);
		GetWorldTimerManager().SetTimer(HealthRecoveryTimer, this, &APro4Character::RecoverPlayerHealthOnServer, 1.0f, true, 5.0f);
	}
}

bool APro4Character::PlayerHealthGetDamagedOnServer_Validate(float Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Processing Player get Damage On Server");

	if (Damage >= 100.0f || Damage < 0.0f)
	{
		return false;
	}
	
	return true;
}

void APro4Character::GetDamaged(float Damage)
{
	PlayerHealthGetDamagedOnServer(Damage);
}

void APro4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APro4Character, CurrentHP);
	DOREPLIFETIME(APro4Character, CurrentAP);
}


#pragma endregion