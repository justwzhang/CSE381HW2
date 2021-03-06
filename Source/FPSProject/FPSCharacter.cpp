// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "FPSBlueProjectile.h"
#include <thread>
#include <chrono>
// Sets default values
AFPSCharacter::AFPSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    OverlapOrange = false;
    OverlapBlue = false;
    // Create a first person camera component.
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FPSCameraComponent != nullptr);

    // Attach the camera component to our capsule component.
    FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

    // Position the camera slightly above the eyes.
    FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

    // Enable the Pawn to control camera rotation.
    FPSCameraComponent->bUsePawnControlRotation = true;

    // Create a first person mesh component for the owning player.
    FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    check(FPSMesh != nullptr);

    // Only the owning player sees this mesh.
    FPSMesh->SetOnlyOwnerSee(true);

    // Attach the FPS mesh to the FPS camera.
    FPSMesh->SetupAttachment(FPSCameraComponent);

    // Disable some environmental shadowing to preserve the illusion of having a single mesh.
    FPSMesh->bCastDynamicShadow = false;
    FPSMesh->CastShadow = false;

    // The owning player doesn't see the regular (third-person) body mesh.
    GetMesh()->SetOwnerNoSee(true);

    // declare trigger capsule
    TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
    TriggerCapsule->InitCapsuleSize(55.f, 96.0f);
    TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
    TriggerCapsule->SetupAttachment(RootComponent);
    TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFPSCharacter::OnOverlapBegin);
    TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AFPSCharacter::OnOverlapEnd);

    HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
    HoldingComponent->SetRelativeLocation(FVector(-100.0f, 0.0f, -40.0f));
    HoldingComponent->SetupAttachment(FPSMesh);

    CurrentHeldProjectile = nullptr;
    CurrentHeldBlueProjectile = nullptr;
    }

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (OverlapOrange) {
        FVector Start = FPSCameraComponent->GetComponentLocation();
        FVector Forward = FPSCameraComponent->GetForwardVector();
        FVector End = ((Forward * 250) + Start);
        FHitResult Hit;
        FComponentQueryParams DefaultComponentQueryParams;
        FCollisionResponseParams DefaultResponseParam;
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
        {
            if (Hit.GetActor()->GetClass()->IsChildOf(AFPSProjectile::StaticClass()))
            {
                CurrentHeldProjectile->Pickup();
                //temp->Destroy();
                ballColor = 1;
            }
        }
    }
    if (OverlapBlue) {
        FVector Start = FPSCameraComponent->GetComponentLocation();
        FVector Forward = FPSCameraComponent->GetForwardVector();
        FVector End = ((Forward * 250) + Start);
        FHitResult Hit;
        FComponentQueryParams DefaultComponentQueryParams;
        FCollisionResponseParams DefaultResponseParam;
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
        {
            if (Hit.GetActor()->GetClass()->IsChildOf(AFPSBlueProjectile::StaticClass()))
            {
                CurrentHeldBlueProjectile->Pickup();
                //temp->Destroy();
                ballColor = 2;
            }
        }
    }

}
void AFPSCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp && ballColor == 0)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
        if (dynamic_cast<AFPSProjectile*>(OtherActor) != nullptr) {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Is A Projectile"));
            OverlapOrange = true;
            AFPSProjectile* temp = dynamic_cast<AFPSProjectile*>(OtherActor);
            CurrentHeldProjectile = temp;
            FVector Start = FPSCameraComponent->GetComponentLocation();
            FVector Forward = FPSCameraComponent->GetForwardVector();
            FVector End = ((Forward * 250) + Start);
            FHitResult Hit;
            FComponentQueryParams DefaultComponentQueryParams;
            FCollisionResponseParams DefaultResponseParam;
            if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
            {
                if (Hit.GetActor()->GetClass()->IsChildOf(AFPSProjectile::StaticClass()))
                {
                    CurrentHeldProjectile->Pickup();
                    //temp->Destroy();
                    ballColor = 1;
                }
            }
        }
        if (dynamic_cast<AFPSBlueProjectile*>(OtherActor) != nullptr) {
            OverlapBlue = true;
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Is A Projectile"));
            AFPSBlueProjectile* temp = dynamic_cast<AFPSBlueProjectile*>(OtherActor);
            CurrentHeldBlueProjectile = temp;
            FVector Start = FPSCameraComponent->GetComponentLocation();
            FVector Forward = FPSCameraComponent->GetForwardVector();
            FVector End = ((Forward * 250) + Start);
            FHitResult Hit;
            FComponentQueryParams DefaultComponentQueryParams;
            FCollisionResponseParams DefaultResponseParam;
            if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
            {
                if (Hit.GetActor()->GetClass()->IsChildOf(AFPSBlueProjectile::StaticClass()))
                {
                    CurrentHeldBlueProjectile->Pickup();
                    //temp->Destroy();
                    ballColor = 2;
                }
            }
            
        }
    }
}

void AFPSCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
            OverlapOrange = false;
            OverlapBlue = false;
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
        //if (ballColor == 0) {
        //    OverlapOrange = false;
        //    OverlapBlue = false;
        //    CurrentHeldProjectile = nullptr;
        //    CurrentHeldBlueProjectile = nullptr;
        //    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
        //}
        //if (ballColor == 1) {
        //    OverlapOrange = false;
        //    OverlapBlue = false;
        //    CurrentHeldBlueProjectile = nullptr;
        //}
        //if (ballColor == 2) {
        //    OverlapOrange = false;
        //    OverlapBlue = false;
        //    CurrentHeldProjectile = nullptr;
        //}
    }
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (GEngine)
    {

    }
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up "movement" bindings.
    PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

    // Set up "look" bindings.
    PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

    // Set up "action" bindings.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

   

void AFPSCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
    bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
    bPressedJump = false;
}

void AFPSCharacter::Fire()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CalledFire")); //for debugging
    // Attempt to fire a projectile.
    if (ProjectileClass && ballColor == 1)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ProjectileClass")); //for debugging
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // Spawn the projectile at the muzzle.
            AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                CurrentHeldProjectile->Destroy();
                CurrentHeldProjectile = nullptr;
                // Set the projectile's initial trajectory.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
                ballColor = 0; //for not allowing the ball to be thrown again
            }
        }
    }
    if (BlueProjectileClass && ballColor == 2)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ProjectileClass")); //for debugging
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            
            // Spawn the projectile at the muzzle.
            AFPSBlueProjectile* Projectile = World->SpawnActor<AFPSBlueProjectile>(BlueProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                CurrentHeldBlueProjectile->Destroy();
                CurrentHeldBlueProjectile = nullptr;
                // Set the projectile's initial trajectory.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
                ballColor = 0; //for not allowing the ball to be thrown
            }
        }
    }
}