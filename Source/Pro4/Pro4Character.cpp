// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Character.h"

// Sets default values
APro4Character::APro4Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	CameraSetting();
	MovementSetting();
	//WeaponSetting();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Mannequin(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
}

// Called when the game starts or when spawned
void APro4Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void APro4Character::CameraSetting()
{
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

void APro4Character::MovementSetting()
{
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	CurrentCharacterState = CharacterState::Stand;
}

//void APro4Character::WeaponSetting()
//{
	//SetWeaponMode(WeaponMode::Disarming);
//}

// Called every frame
void APro4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APro4Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APro4Character::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APro4Character::Fire);
	PlayerInputComponent->BindAction(TEXT("Sit"), EInputEvent::IE_Pressed, this, &APro4Character::Sitting);
	PlayerInputComponent->BindAction(TEXT("Lie"), EInputEvent::IE_Pressed, this, &APro4Character::Lying);
	PlayerInputComponent->BindAction(TEXT("Key1"), EInputEvent::IE_Pressed, this, &APro4Character::EquipMain1);
	PlayerInputComponent->BindAction(TEXT("Key2"), EInputEvent::IE_Pressed, this, &APro4Character::EquipMain2);
	PlayerInputComponent->BindAction(TEXT("Key3"), EInputEvent::IE_Pressed, this, &APro4Character::EquipSub);
	PlayerInputComponent->BindAction(TEXT("Key4"), EInputEvent::IE_Pressed, this, &APro4Character::EquipATW);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APro4Character::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APro4Character::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APro4Character::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APro4Character::Turn);
}

void APro4Character::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}

void APro4Character::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void APro4Character::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void APro4Character::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void APro4Character::Jump()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Stand:
		Super::Jump();
		break;
	case CharacterState::Sit:
		UE_LOG(Pro4, Log, TEXT("Stand."));
		CurrentCharacterState = CharacterState::Stand;
		break;
	case CharacterState::Lie:
		UE_LOG(Pro4, Log, TEXT("Stand."));
		CurrentCharacterState = CharacterState::Stand;
		break;
	}
	

}

void APro4Character::Fire()
{
	switch (CurrentWeaponMode)
	{
	case WeaponMode::Main1:
		UE_LOG(Pro4, Log, TEXT("Fire1."));
		break;
	case WeaponMode::Main2:
		UE_LOG(Pro4, Log, TEXT("Fire2."));
		break;
	case WeaponMode::Sub:
		UE_LOG(Pro4, Log, TEXT("FireSub."));
		break;
	case WeaponMode::ATW:
		UE_LOG(Pro4, Log, TEXT("Throw."));
		break;
	case WeaponMode::Disarming:
		UE_LOG(Pro4, Log, TEXT("Fist."));
		break;
	}
	UE_LOG(Pro4, Log, TEXT("Attack"));
}

void APro4Character::Sitting()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Stand:
		UE_LOG(Pro4, Log, TEXT("Sit."));
		CurrentCharacterState = CharacterState::Sit;
		break;
	case CharacterState::Sit:
		UE_LOG(Pro4, Log, TEXT("Stand."));
		CurrentCharacterState = CharacterState::Stand;
		break;
	case CharacterState::Lie:
		UE_LOG(Pro4, Log, TEXT("Sit."));
		CurrentCharacterState = CharacterState::Sit;
		break;
	}
}

void APro4Character::Lying()
{
	switch (CurrentCharacterState)
	{
	case CharacterState::Stand:
		UE_LOG(Pro4, Log, TEXT("Lie."));
		CurrentCharacterState = CharacterState::Lie;
		break;
	case CharacterState::Sit:
		UE_LOG(Pro4, Log, TEXT("Lie."));
		CurrentCharacterState = CharacterState::Lie;
		break;
	case CharacterState::Lie:
		UE_LOG(Pro4, Log, TEXT("Stand."));
		CurrentCharacterState = CharacterState::Stand;
		break;
	}
}

void APro4Character::EquipMain1()
{
	if (CurrentWeaponMode == WeaponMode::Main1)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		CurrentWeaponMode = WeaponMode::Disarming;
	}
	else
	{
		UE_LOG(Pro4, Log, TEXT("EquipMain1."));
		CurrentWeaponMode = WeaponMode::Main1;
	}
}

void APro4Character::EquipMain2()
{
	if (CurrentWeaponMode == WeaponMode::Main2)
	{
		UE_LOG(Pro4, Log, TEXT("Disarming."));
		CurrentWeaponMode = WeaponMode::Disarming;
	}
	else
	{
		UE_LOG(Pro4, Log, TEXT("EquipMain2."));
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
		UE_LOG(Pro4, Log, TEXT("EquipSub."));
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