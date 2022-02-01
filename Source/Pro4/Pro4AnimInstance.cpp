// Fill out your copyright notice in the Description page of Project Settings.


#include "Pro4AnimInstance.h"
#include "Pro4Character.h"

UPro4AnimInstance::UPro4AnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsCrouch = false;
	IsProne = false;
	IsRun = false;
}

void UPro4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APro4Character>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsCrouch = Character->GetMovementComponent()->IsCrouching();
			IsProne = Character->IsProning();
			IsRun = Character->IsRunning();
		}
	}
}