// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseItem.h"
#include "../UserInterface/ItemNameWidget.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AGrenade.generated.h"

/**
 * 
 */
UCLASS()
class PRO4_API AAGrenade : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AAGrenade();

	enum class GrenadeType : int32
	{
		Grenade,	// 수류탄
		Flash,		// 섬광탄
		Smoke,		// 연막탄
		MAX
	};

	void ViewItemName();

	UFUNCTION(NetMulticast, Reliable)
	void PlayGrenadeSound();

	UFUNCTION(NetMulticast, Reliable)
	void SetStateToExplosion();

	UFUNCTION(NetMulticast, Reliable)
	void ThrowGrenade(const FString& GrenadeType, UStaticMesh* GrenadeMesh);

	UFUNCTION()
	void GrenadeExplosion();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetUp(UStaticMesh* SM_Grenade, const FString& _ItemName, uint16 _ItemNum, UParticleSystem* ParticleSystem);

	void SetSimulatePhysics(const FVector& ThrowDirection);

#pragma region Get_Set
	UParticleSystem* GetGrenadeParticle()
	{
		return P_GrenadeParticle;
	}
	
	UParticleSystem* GetFlashParticle()
	{
		return P_FlashParticle;
	}

	UParticleSystem* GetSmokeParticle()
	{
		return P_SmokeParticle;
	}

	/* ItemName */
	void SetItemName(FString _Name)
	{
		ItemName = _Name;
		WBP_NameWidget->SetItemName(ItemName);
	}

	FString GetItemName()
	{
		return ItemName;
	}

	/* ItemNum */
	void SetItemNum(uint16 _Num)
	{
		ItemNum = _Num;
	}

	uint16 GetItemNum()
	{
		return ItemNum;
	}

#pragma endregion

	virtual void BeginPlay();

protected:
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	void RandomSpawn(int32 Random);

	UFUNCTION()
	void SetGrenadeExplosion();

	GrenadeType CurrentGrenade;
	FString TemporaryName;
	UStaticMesh* SM_GrenadeItem;
	
	USoundCue* ExplosionSound;
	USoundCue* SmokeSound;
	USoundCue* FlashSound;

	/* 현재 가지고 있는 사운드 */
	USoundCue* CurrentSound;

	/* Grenade 아이템이 가질 수 있는 파티클 */
	UParticleSystem* P_GrenadeParticle;
	UParticleSystem* P_SmokeParticle;
	UParticleSystem* P_FlashParticle;

	/* 아이템이 가지고 있는 현재 파티클 */
	UParticleSystem* CurrentParticle;

	UPROPERTY(VisibleAnywhere, Category = "GrenadeProejctile")
	UProjectileMovementComponent* GrenadeProjectile;

	UPROPERTY(VisibleAnywhere, Category = "GrenadeParticle")
	UParticleSystemComponent* GrenadeParticle;

	UPROPERTY(VisibleAnywhere, Category = "GrenadeSound")
	UAudioComponent* AC;

	UPROPERTY(Replicated)
	FString ItemName;

	UPROPERTY(Replicated)
	uint16 ItemNum;

	class UItemNameWidget* WBP_NameWidget;

	bool bIsObservable;

	FTimerHandle SetExplosionTimer;
};
