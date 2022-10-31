// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "Engine/DecalActor.h"
#include "D_EncroachField.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AD_EncroachField : public ADecalActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AD_EncroachField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void NotifyActorBeginOverlap(AActor* Act) override;
	void NotifyActorEndOverlap(AActor* Act) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Pivot)
	USphereComponent* EncroachPivotComponent;
};
