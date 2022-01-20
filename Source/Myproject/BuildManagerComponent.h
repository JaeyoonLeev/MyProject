// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UBuildManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ToggleBuildMode();

	UFUNCTION(BlueprintCallable)
	void RequestBuild();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	float GridSize = 525.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	float BuildDistance = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	float FloorHeight = 130.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	TSubclassOf<class ABuildable> WoodWall;

private:
	FVector getNextBuildLocation() const;
	FRotator getNextBuildRotation() const;

	bool isBuilding;
	class UCameraComponent *Camera;

	class ABuildable* currentBuild;
};
