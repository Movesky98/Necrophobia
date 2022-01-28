// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4Character.h"
#include "DrawDebugHelpers.h"

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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PCharacter"));

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

	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 50.0f);
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

			if (Interactable->ActorHasTag(TEXT("Item")))
			{
				UE_LOG(Pro4, Log, TEXT("Get %s"), *Interactable->GetName());
				
				Interactable->Destroy();
			}
		}
	}
}
