// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h>
#include "JumpingAIController.generated.h"


/**
 * 
 */
UCLASS()
class FPSPROJECT_API AJumpingAIController : public AAIController
{
	GENERATED_BODY()
		UBehaviorTreeComponent* BehaviorTreeComp;
		UBlackboardComponent* BlackboardComp;

		UPROPERTY(EditDefaultsOnly, Category = AI)
		FName LocationToGoKey;

		TArray<AActor*> TargetPoints;
		
		virtual void OnPossess(APawn* ThePawn) override;
		ACharacter* AICharacter;

public:
	AJumpingAIController();
	FORCEINLINE UBlackboardComponent* GetBackboardComponent() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*> GetTargetPoints() const { return TargetPoints; }
	int32 CurrentPoint;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
