// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Myproject.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDeleagate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeleagate);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetPawnSpeed(float NewPawnSpeed) { CurrentPawnSpeed = NewPawnSpeed; }
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 SectionIndex);

public:
	FOnNextAttackCheckDeleagate OnNextAttackCheck;
	FOnAttackHitCheckDeleagate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 SectionIndex);


private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};
