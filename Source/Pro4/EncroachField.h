// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pro4.h"
#include "GameFramework/Actor.h"
#include "EncroachField.generated.h"


UCLASS()
class PRO4_API AEncroachField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEncroachField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* Act) override;
	void NotifyActorEndOverlap(AActor* Act) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Pivot)
		USphereComponent* EncroachPivotComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Pivot)
		UStaticMeshComponent* EncroachMeshComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
