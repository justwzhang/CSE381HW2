// Copyright Epic Games, Inc.All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include <FPSProject/JumpingAICharacter.h>

// Sets default values
AFPSProjectile::AFPSProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Holding = false;
    Gravity = true;

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
        TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Capsule"));
        TriggerComponent ->InitSphereRadius(20.0f);
        TriggerComponent->SetCollisionProfileName(TEXT("Trigger"));
        TriggerComponent->SetupAttachment(RootComponent);
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
        ProjectileMovementComponent->Bounciness = 0.7f;
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
        static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/SphereMaterial.SphereMaterial'"));
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
    CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
    Super::BeginPlay();
    MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();
    TArray<USceneComponent*> Components;
    MyCharacter->GetComponents(Components);
    if (Components.Num() > 0)
    {
        for (auto& Comp : Components)
        {
            if (Comp->GetName() == "HoldingComponent")
            {
                HoldingComp = Cast<USceneComponent>(Comp);
            }
        }
    }
}

void AFPSProjectile::Pickup()
{
    Holding = !Holding;
    Gravity = !Gravity;
    ProjectileMeshComponent ->SetEnableGravity(Gravity);
    ProjectileMeshComponent->SetSimulatePhysics(Holding ? false : true);
    ProjectileMeshComponent->SetCollisionEnabled(Holding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
    if (HoldingComp && Holding)
    {
        ProjectileMeshComponent->AttachToComponent(HoldingComp, FAttachmentTransformRules::KeepWorldTransform);
        SetActorLocation(HoldingComp->GetComponentLocation());
    }
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HoldingComp && Holding)
    {
        SetActorLocation(HoldingComp->GetComponentLocation());
    }

}

// Function that initializes the projectile's velocity in the shoot direction.
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("In OnHit Event")); //for debugging
    //the only other thing that will be simulating physics are the things in the hoops so the code for
    //scoring should be in this if
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics() && dynamic_cast<AJumpingAICharacter*>(OtherActor))
    {
        //OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
        //Code for Scoring
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("test")); //for debugging
        Destroy();
    }

    
}
