#pragma once

#include "Pro4.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Pro4Projectile.generated.h"

UCLASS()
class PRO4_API APro4Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APro4Projectile();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void ProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	UParticleSystemComponent* ProjectileParticle;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Particle")
	UParticleSystem* Particle_GroundDirt;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Particle")
	UParticleSystem* Particle_Ceramic;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Particle")
	UParticleSystem* Particle_Wood;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Particle")
	UParticleSystem* Particle_Blood;

	void FireInDirection(const FVector& ShootDirection);
};