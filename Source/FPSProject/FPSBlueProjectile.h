// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSBlueProjectile.generated.h"

UCLASS()
class FPSPROJECT_API AFPSBlueProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlueProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    //1:orange, 2:blue
    int ballColor = 2;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Sphere collision component
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* CollisionComponent;

    // Sphere tigger component
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* TriggerComponent;

    // Projectile movement component
    UPROPERTY(VisibleAnywhere, Category = Movement)
        UProjectileMovementComponent* ProjectileMovementComponent;

    // Projectile mesh
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        UStaticMeshComponent* ProjectileMeshComponent;

    // Projectile material
    UPROPERTY(VisibleDefaultsOnly, Category = Movement)
        UMaterialInstanceDynamic* ProjectileMaterialInstance;

    // Function that initializes the projectile's velocity in the shoot direction.
    void FireInDirection(const FVector& ShootDirection);

    // Function that is called when the projectile hits something.
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};