// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildManagerComponent.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Buildable.h"

// Sets default values for this component's properties
UBuildManagerComponent::UBuildManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	isBuilding = false;
	// ...
}


// Called when the game starts
void UBuildManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (Camera == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Unable to find player camera"));
	}
	// ...

}


// Called every frame
void UBuildManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isBuilding)
	{
		FVector location = getNextBuildLocation();
		FRotator rotation = getNextBuildRotation();

		//DrawDebugBox(GetWorld(), location, FVector(100, 100, 100), rotation.Quaternion(), FColor::Red, false, 0, 0, 10);

		if (currentBuild == nullptr)
		{ //Create currentBuild
			currentBuild = GetWorld()->SpawnActor<ABuildable>(WoodWall, location, rotation, FActorSpawnParameters{});
		}
		else
		{ // Update location and rotation
			
			currentBuild->SetActorLocationAndRotation(location, rotation);
		}
	}
	else if (currentBuild != nullptr)
	{//Destroy current Build
		currentBuild->Destroy();
		currentBuild = nullptr;
	}
	// ...
}

void UBuildManagerComponent::ToggleBuildMode()
{
	isBuilding = !isBuilding;
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green,
		FString::Printf(TEXT("UBulidManagerComponent::ToggleBuildMode() -> %d"),isBuilding));

}

void UBuildManagerComponent::RequestBuild()
{
	if (!isBuilding || currentBuild == nullptr) {
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("UBulidManagerComponent::RequestBuild()"));
	currentBuild->Build();
	currentBuild = nullptr;
}

FVector UBuildManagerComponent::getNextBuildLocation() const
{
	FVector directionVector = Camera->GetForwardVector() * BuildDistance;
	directionVector += GetOwner()->GetActorLocation();
	return FVector(
		FMath::GridSnap(directionVector.X, GridSize),
		FMath::GridSnap(directionVector.Y, GridSize),
		FloorHeight
	);
}

FRotator UBuildManagerComponent::getNextBuildRotation() const
{
	FRotator rotation = Camera->GetComponentRotation();
	return FRotator(0, FMath::GridSnap(rotation.Yaw, 90.f),0);
}

