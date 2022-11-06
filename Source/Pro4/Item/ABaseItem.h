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

	/* 무기종류 */
	enum class BaseItemType : int32
	{
		Weapon,		// 무기
		Armor,		// 방어구
		Grenade,	// 투척무기
		Recovery,	// 회복 아이템
		Ammo,		// 탄창
		Vaccine,	// 탈출용 백신
		MAX
	};
	
	BaseItemType ItemType;

	/* 아이템을 그려낼 SkeletalMeshComponent */
	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	USkeletalMeshComponent* SK_Mesh;

	/* 아이템을 그려낼 StaticMeshComponent */
	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	UStaticMeshComponent* BoxMesh;

	/* 아이템의 이름을 보여줄 UI */
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* NameWidget;
};
