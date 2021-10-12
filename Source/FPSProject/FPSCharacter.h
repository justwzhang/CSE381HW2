// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsultComponent.h"
#include "FPSProjectile.h"
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

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UIComponent* PlayerInputComponent) override;

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

};