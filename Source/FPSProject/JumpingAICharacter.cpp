// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpingAICharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AJumpingAICharacter::AJumpingAICharacter()
{
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionCapsule->InitCapsuleSize(55.0f, 150.0f);
	CollisionCapsule->SetupAttachment(RootComponent);;
}

// Called when the game starts or when spawned
void AJumpingAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AJumpingAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

