// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pro4.h"
#include "GameFramework/Actor.h"
#include "ABaseItem.generated.h"

UCLASS()
class PRO4_API AABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABaseItem();

#pragma region ItemTypeDeclaration
	enum class BaseItemType : int32
	{
		Weapon,		// 무기
		Armor,		// 방어구
		Grenade,	// 투척무기
		Recovery,	// 회복 아이템
		Ammo,		// 탄창
		Parts,		// 파츠, 부품
		MAX
	};
	
	BaseItemType ItemType;

	enum class RecoveryType : int32
	{
		Antidote,	// 해독제
		Bandage,	// 붕대
		Kit,		// 회복키트
		MAX
	};

#pragma endregion

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	USkeletalMeshComponent* SK_Mesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = SphereCollision)
	USphereComponent* SphereCollision;
};
