// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectNextPoint.h"
#include "FPSProject.h"
#include "JumpingAIController.h"
#include "JusmpingAITargetPoint.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>

EBTNodeResult::Type USelectNextPoint::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AJumpingAIController* controller = Cast<AJumpingAIController>(Comp.GetAIOwner());
	if (controller) {
		UBlackboardComponent* BBComp = controller->GetBackboardComponent();
		AJusmpingAITargetPoint* CurrentPoint = Cast<AJusmpingAITargetPoint>(BBComp->GetValueAsObject("LocationToGo"));
		TArray<AActor*> PointsArray = controller->GetTargetPoints();
		AJusmpingAITargetPoint* NextPoint;
		controller->CurrentPoint = rand() % PointsArray.Num();
		NextPoint = Cast<AJusmpingAITargetPoint>(PointsArray[controller->CurrentPoint]);
		/*if (controller->CurrentPoint != PointsArray.Num() -1) {
			NextPoint = Cast<AJusmpingAITargetPoint>(PointsArray[++controller->CurrentPoint]);
		}
		else {
			NextPoint = Cast<AJusmpingAITargetPoint>(PointsArray[0]);
			controller->CurrentPoint = 0;
		}*/
		BBComp->SetValueAsObject("LocationToGo", NextPoint);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
