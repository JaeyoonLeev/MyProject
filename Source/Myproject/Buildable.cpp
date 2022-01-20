// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable.h"
#include "Components/BoxComponent.h"
#include "Engine/CollisionProfile.h"

// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(RootComponent);

	BuildMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BuildMesh"));
	BuildMesh->SetupAttachment(RootComponent);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	PreviewDestroyAnimation = CreateDefaultSubobject<UAnimationAsset>(TEXT("PreviewDestroyAnimation"));
	BuildAnimation = CreateDefaultSubobject<UAnimationAsset>(TEXT("BuildAnimation"));


}

void ABuildable::Build()
{
	BuildMesh->SetVisibility(true);
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	BuildMesh->PlayAnimation(BuildAnimation, false);
	PreviewMesh->PlayAnimation(PreviewDestroyAnimation, false);

}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

