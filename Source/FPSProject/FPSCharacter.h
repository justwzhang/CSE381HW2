// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "FPSBlueProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AFPSCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Projectile class to spawn.
    UPROPERTY(EditAnywhere, Category = Projectile)
    TSubclassOf<class AFPSProjectile> ProjectileClass;
    UPROPERTY(EditAnywhere, Category = Projectile)
    TSubclassOf<class AFPSBlueProjectile> BlueProjectileClass;

    /*UPROPERTY(EditDefaultsOnly, Category = Spawning)
    TSubclassOf<class AFPSProjectile> ProjectileClassSpawn;
    UPROPERTY(EditDefaultsOnly, Category = Spawning)
    TSubclassOf<class AFPSBlueProjectile> BlueProjectileClassSpawn;*/


public:

    //for checking if the player is holding a ball
    //bool holdingBall = false;
    //for checking which color the ball is 0:none, 1:orange, 2:blue
    int ballColor = 0;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Handles input for moving forward and backward.
    UFUNCTION()
        void MoveForward(float Value);

    // Handles input for moving right and left.
    UFUNCTION()
        void MoveRight(float Value);

    // Sets jump flag when key is pressed.
    UFUNCTION()
        void StartJump();

    // Clears jump flag when key is released.
    UFUNCTION()
        void StopJump();

    // Function that fires projectiles.
    UFUNCTION()
        void Fire();

    // FPS camera
    UPROPERTY(VisibleAnywhere)
        UCameraComponent* FPSCameraComponent;

    // First-person mesh (arms), visible only to the owning player.
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        USkeletalMeshComponent* FPSMesh;

    // Gun muzzle offset from the camera location.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector MuzzleOffset;

    // create trigger capsule
    UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
        class UCapsuleComponent* TriggerCapsule;

    UFUNCTION()
        void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // declare overlap end function
    UFUNCTION()
        void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /*UFUNCTION()
        void SpawnObject(const FVector loc, const FRotator rot);*/

    UPROPERTY(EditAnywhere)
    class AFPSProjectile* CurrentHeldProjectile;
    UPROPERTY(EditAnywhere)
    class AFPSBlueProjectile* CurrentHeldBlueProjectile;
    
    /** Holding Component */
    UPROPERTY(EditAnywhere)
    USceneComponent* HoldingComponent;
};