// Fill out your copyright notice in the Description page of Project Settings.


#include "MyKraken.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "MyCharacter.h"

// Sets default values
AMyKraken::AMyKraken()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectPlayerCollisionSphere =
		CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	DetectPlayerCollisionSphere->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AMyKraken::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyKraken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyKraken::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

USphereComponent* AMyKraken::GetDetectPlayerCollisionSphere()
{
	return DetectPlayerCollisionSphere;
}

