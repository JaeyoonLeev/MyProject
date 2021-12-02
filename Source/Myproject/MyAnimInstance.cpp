// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Medea/animation/Medea_Sword_Action.Medea_Sword_Action"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;

	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//auto Pawn = TryGetPawnOwner();
	//if (::IsValid(Pawn))
	//{
	//	CurrentPawnSpeed = Pawn->GetVelocity().Size();
	//}
	
	auto Pawn = TryGetPawnOwner();
	if(!IsDead) return;

	if (::IsValid(Pawn))
	{
		//CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	Montage_Play(AttackMontage, 1.0f);

}

void UMyAnimInstance::JumpToAttackMontageSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageSectionName(SectionIndex);
	Montage_JumpToSection(GetAttackMontageSectionName(SectionIndex), AttackMontage);
}

void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 SectionIndex)
{

	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

//FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
//{
//	//return FName(*FString::Printf("Attack%d"), Section);
//}