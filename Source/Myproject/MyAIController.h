// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Myproject.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;

	//UPROPERTY()
	//class UBlackboardComponent* BBComponent;

	//UPROPERTY()
	//class UBehaviorTreeComponent* BTComponent;

};
