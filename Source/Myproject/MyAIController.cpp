// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"

AMyAIController::AMyAIController()
{
	//RepeatInterval = 3.0f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/BluePrints/Monster/BT_Zombie.BT_Zombie'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}



	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/BluePrints/Monster/BB_Zombie.BB_Zombie'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

	//BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
	//BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
}


void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//AZombie* Zombie = Cast<AZombie>(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMyAIController::OnRepeatTimer, 3.f, true);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
			//TODO
		}
	}

	//if (Zombie->BehaviorTree && Zombie->BehaviorTree->BlackboardAsset)
	//{
	//	BBComponent->InitializeBlackboard(*(Zombie->BehaviorTree->BlackboardAsset));
	//	BTComponent->StartTree(*(Zombie->BehaviorTree));
	//}

}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);

}

void AMyAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		//UNavigationSystemV1::SimpleMoveToLocation(this, NextLocation.Location);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogClass, Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}

}
