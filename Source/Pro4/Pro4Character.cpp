// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Character.h"
#include "Pro4AnimInstance.h"
#include "NecrophobiaGameInstance.h"
#include "UserInterface/PlayerMenu.h"

#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APro4Character::APro4Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FName WeaponSocket(TEXT("Hand_rSocket"));
	bReplicates = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

	MapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MAPSPRINGARM"));
	MapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MAPCAPTURE"));

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

	if (GetMesh()->DoesSocketExist(WeaponSocket)) {

		UE_LOG(Pro4, Log, TEXT("WeaponSocket has exist"))

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Weapon(TEXT("/Game/Weapon/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SM_AR4_X.SM_AR4_X"));
		if (SM_Weapon.Succeeded())
		{
			Weapon->SetStaticMesh(SM_Weapon.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
}

// Called when the game starts or when spawned
void APro4Character::BeginPlay()
{
	Super::BeginPlay();
	
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

	CurrentHP += 1.0f;
	if (CurrentHP >= 90.0f) 
	{
		CurrentHP = 10.0f;
	}

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
	}
}

void APro4Character::EquipMain2()
{
	if (CurrentWeaponMode == WeaponMode::Main2)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		Equipflag = 0;
		CurrentWeaponMode = WeaponMode::Disarming;
	}
	else
	{
		if (IsEquipping) return;
		UE_LOG(Pro4, Log, TEXT("EquipMain2."));
		Equipflag = 1;
		Pro4Anim->PlayEquipMontage();
		Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->EquipMontage);
		IsEquipping = true;
		CurrentWeaponMode = WeaponMode::Main2;
	}
}

void APro4Character::EquipSub()
{
	if (CurrentWeaponMode == WeaponMode::Sub)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		CurrentWeaponMode = WeaponMode::Disarming;
	}
	else
	{
		if (IsEquipping) return;
		UE_LOG(Pro4, Log, TEXT("EquipSub."));
		Equipflag = 2;
		Pro4Anim->PlayEquipMontage();
		Pro4Anim->Montage_JumpToSection(FName("2"), Pro4Anim->EquipMontage);
		IsEquipping = true;
		CurrentWeaponMode = WeaponMode::Sub;
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

/// <summary>
////////////////////////////////////////////////////// 캐릭터 공격 코드 ////////////////////////////////////////////////////////////
/// </summary>

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
			Fire();
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

		FVector MuzzleLocation = Weapon->GetComponentLocation();
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();

		if (CurrentWeaponMode == WeaponMode::Main1 || CurrentWeaponMode == WeaponMode::Main2) // 주무기일 때의 총알 발사 (탄창 상태 반영 안함)
		{
			if (IsZoom)
			{
				Pro4Anim->PlayAttackMontage();
				Pro4Anim->Montage_JumpToSection(FName("2"), Pro4Anim->AttackMontage);
				UE_LOG(Pro4, Log, TEXT("2."));
			}
			else
			{
				Pro4Anim->PlayAttackMontage();
				Pro4Anim->Montage_JumpToSection(FName("1"), Pro4Anim->AttackMontage);
				UE_LOG(Pro4, Log, TEXT("1."));
			}

			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				APro4Projectile* Projectile = World->SpawnActor<APro4Projectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
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

/// <summary>
////////////////////////////////////////////////////// 캐릭터 공격 코드 ////////////////////////////////////////////////////////////
/// </summary>



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
				Server_DestroyItem(Interactable);
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