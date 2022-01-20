// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Myproject.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal = 0 UMETA(Display = "Normal"),
	Chase = 1 UMETA(Display = "Chase"),
	Battle = 2 UMETA(Display = "Battle"),
	Dead = 3 UMETA(Display = "Dead")
};

UCLASS()
class MYPROJECT_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float RunSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float AttackRange = 100.0f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UFUNCTION()
		void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		void OnHearNoise(APawn* Pawn, const FVector& Location, float Volume);
};
