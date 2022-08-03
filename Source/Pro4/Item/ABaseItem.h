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

public:
#pragma region ItemTypeDeclaration
	enum class BaseItemType : uint8
	{
		Weapon,		// ����
		Armor,		// ��
		Grenade,	// ��ô����
		Recovery,	// ȸ�� ������
		Ammo,		// źâ
		Parts,		// ����, ��ǰ
	};
	
	BaseItemType ItemType;

	enum class RecoveryType : uint8 
	{
		Antidote,	// �ص���
		Bandage,	// �ش�
		Kit,		// ȸ��ŰƮ
	};

#pragma endregion

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = SphereCollision)
	USphereComponent* SphereCollision;
};
