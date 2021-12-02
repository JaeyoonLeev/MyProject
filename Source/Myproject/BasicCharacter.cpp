// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CharM_Ram(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram"));
	if (SK_CharM_Ram.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CharM_Ram.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);


	//애니메이션블루프린트를 불러올때는 _C를 붙여야함
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	InputComponent->BindAxis(TEXT("UpDown"), this, &ABasicCharacter::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ABasicCharacter::LeftRight);
	InputComponent->BindAxis("LookUp", this, &ABasicCharacter::LookUp);
	InputComponent->BindAxis("Turn", this, &ABasicCharacter::Turn);
}

void ABasicCharacter::UpDown(float NewAxisValue)
{
	if (NewAxisValue == 0.f)
		return;

	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void ABasicCharacter::LeftRight(float NewAxisValue)
{
	if (NewAxisValue == 0.f)
		return;

	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

void ABasicCharacter::LookUp(float NewAxisValue)
{
	if (NewAxisValue != 0.0)
	{
		AddControllerPitchInput(-NewAxisValue);

	}
}

void ABasicCharacter::Turn(float NewAxisValue)
{
	if (NewAxisValue != 0.0)
	{
		AddControllerYawInput(NewAxisValue);

	}
}

