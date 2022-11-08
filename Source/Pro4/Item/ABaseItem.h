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

	/* �������� */
	enum class BaseItemType : int32
	{
		Weapon,		// ����
		Armor,		// ��
		Grenade,	// ��ô����
		Recovery,	// ȸ�� ������
		Ammo,		// źâ
		Vaccine,	// Ż��� ���
		MAX
	};
	
	BaseItemType ItemType;

	/* �������� �׷��� SkeletalMeshComponent */
	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	USkeletalMeshComponent* SK_Mesh;

	/* �������� �׷��� StaticMeshComponent */
	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	UStaticMeshComponent* BoxMesh;

	/* �������� �̸��� ������ UI */
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* NameWidget;
};
