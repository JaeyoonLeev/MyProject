// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Buildable.generated.h"

UCLASS()
class MYPROJECT_API ABuildable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Building)
	USkeletalMeshComponent* BuildMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	class UBoxComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UAnimationAsset* BuildAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Building)
	UAnimationAsset* PreviewDestroyAnimation;

	UFUNCTION(BlueprintCallable, Category = Building)
	void Build();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
