// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Heli_AH64D.generated.h"

UCLASS()
class PRO4_API AHeli_AH64D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeli_AH64D();

	UFUNCTION(BlueprintCallable)
	void CallEscape();

	/* 메인 로터의 스피드를 설정하는 함수 (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetMainRotorSpeed(float MainRotorSpeed);

	/* 메인 로터의 플랩 각도를 설정하는 함수 (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetMainRotorFlapsAngle(float MainRotorFlapsAngle);

	/* 헬리콥터 꼬리쪽 로터의 스피드를 설정하는 함수 (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetTailRotorSpeed(float TailRotorSpeed);

	/*헬리콥터 꼬리쪽 플랩 각도를 설정하는 함수 (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetTailRotorFlapsAngle(float TailRotorFlapsAngle);

	/* 헬리콥터 문의 각도를 설정하는 함수 (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetDoorsAngle(float DoorsAngle);

	/* 탈출용 헬러콥터가 생성되었을 때,  (블루프린트에서 구현됨.) */
	UFUNCTION(BlueprintImplementableEvent)
	void SetHelicopterSetting(FVector TargetLocation, FVector SpawnLocation, FRotator TargetRotation);

	UFUNCTION(BlueprintCallable)
	void SetTargetPlayerLocation(FVector Location)
	{
		TargetPlayerLocation = Location;
	}

	FVector GetTargetPlayerLocation()
	{
		return TargetPlayerLocation;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void ActiveEscapeCollision();
	UFUNCTION()
	void CheckEscapeCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MachineGunFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Damaged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* DamagedSmoke;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* EscapeCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Helicopter, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Heli;
	
	class USoundCue* HeliSound;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Escape")
	FVector TargetPlayerLocation;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Escape")
	bool IsReachToPlayerLocation = false;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Escape")
	bool IsBoardTheHelicopter = false;
};
