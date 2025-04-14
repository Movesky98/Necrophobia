// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HumanInputComponent.h"


#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// Sets default values for this component's properties
UHumanInputComponent::UHumanInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UHumanInputComponent::BindInputs(UInputComponent* CharacterInputComponet)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(CharacterInputComponet))
	{


	}
}


// Called when the game starts
void UHumanInputComponent::BeginPlay()
{
	Super::BeginPlay();


}