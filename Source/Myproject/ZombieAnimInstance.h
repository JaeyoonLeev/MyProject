// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Zombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
		EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "State")
		float Speed;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION()
		void AnimNotify_NotifyTest(UAnimNotify* Notify);
	
};
