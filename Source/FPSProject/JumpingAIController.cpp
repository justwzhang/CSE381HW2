// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpingAIController.h"
#include "JumpingAICharacter.h"
#include "JusmpingAITargetPoint.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>

AJumpingAIController::AJumpingAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	LocationToGoKey = "LocationToGo";
	CurrentPoint = 0;
}

void AJumpingAIController::OnPossess(APawn* ThePawn)
{
	Super::OnPossess(ThePawn);

	AJumpingAICharacter* theCharacter = Cast<AJumpingAICharacter>(ThePawn);
	if (theCharacter) {
		AICharacter = theCharacter;
		if (theCharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(theCharacter->BehaviorTree->BlackboardAsset));
		}
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AJusmpingAITargetPoint::StaticClass(), TargetPoints);
		BehaviorTreeComp->StartTree(*theCharacter->BehaviorTree);
	}
}

// Called every frame
void AJumpingAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AICharacter != nullptr) {
		FVector vector = AICharacter->GetVelocity();
		FVector* temp = &vector;
		if ((temp->Size()) <= 0.5) {
			AICharacter->Jump();
		}
	}

}


