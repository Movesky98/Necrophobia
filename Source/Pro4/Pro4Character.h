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

	// void NotifyActorBeginOverlap(AActor* OtherActor) override;

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
		Stand,
		Sit,
		Lie,
	};

	CharacterState CurrentCharacterState = CharacterState::Stand;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override
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
		PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APro4Character::InteractPressed);

		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APro4Character::UpDown);
		PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APro4Character::LeftRight);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APro4Character::LookUp);
		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APro4Character::Turn);
	}

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent *Camera;

	UPROPERTY(VisibleDefaultsOnly, Category=Interact)
	bool bHit;

private:
	void MovementSetting();
	void CameraSetting();
	//void WeaponSetting();

	void Fire();
	void Sitting();
	void Lying();
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
};
