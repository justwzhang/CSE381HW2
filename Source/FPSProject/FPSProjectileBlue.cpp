// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectileBlue.h"

// Sets default values
AFPSProjectileBlue::AFPSProjectileBlue()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // Use a sphere as a simple collision representation.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // Set the sphere's collision radius.
        CollisionComponent->InitSphereRadius(15.0f);
        // Set the root component to be the collision component.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.8f;
        ProjectileMovementComponent->ProjectileGravityScale = 3.0f;
    }

    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Sphere.Sphere'"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }

        static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/SphereMaterialBlue.SphereMaterialBlue'"));
        if (Material.Succeeded())
        {
            ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
        }
        ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
        ProjectileMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
        ProjectileMeshComponent->SetupAttachment(RootComponent);
    }
    // Delete the projectile after 3 seconds.
    //InitialLifeSpan = 3.0f;
    // Set the sphere's collision profile name to "Projectile".
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

    // Event called when component hits something.
    CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectileBlue::OnHit);
}

// Called when the game starts or when spawned
void AFPSProjectileBlue::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AFPSProjectileBlue::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.
void AFPSProjectileBlue::FireInDirection(const FVector & ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AFPSProjectileBlue::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
    /*if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
        Destroy();
    }*/


}

