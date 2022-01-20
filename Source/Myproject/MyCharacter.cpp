// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
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
#include "Components/SceneComponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyCharacterStatComponent.h"
#include "BuildManagerComponent.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	BuildManager = CreateDefaultSubobject<UBuildManagerComponent>(TEXT("BuildManager"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;



	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Medea02(TEXT("/Game/Medea/mesh/SK_Medea02"));
	if (SK_Medea02.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Medea02.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);


	//애니메이션블루프린트를 불러올때는 _C를 붙여야함
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Medea/animation/A_ThirdPerson_AnimBP.A_ThirdPerson_AnimBP_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	FName WeaponSocket(TEXT("ik_hand_sword"));
	//if (GetMesh()->DoesSocketExist(WeaponSocket))
	//{
	//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	//	if (SK_WEAPON.Succeeded())
	//	{
	//		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	//	}

	//	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	//}


	SetControlMode(EControlMode::DIABLO);
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	IsAttacking = false;
	CurrentCombo = 1;
	MaxCombo = 4;
	AttackEndComboState();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
	AttackRange = 200.0f;
	AttackRadius = 50.0f;


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FName WeaponSocket(TEXT("ik_hand_sword"));

	auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	//auto CurWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//}
}

//void AMyCharacter::SetControlMode(int32 ControlMode)
//{
//	if (ControlMode == 0)
//	{
//		SpringArm->TargetArmLength = 450.0f;
//		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
//		SpringArm->bUsePawnControlRotation = true;
//		SpringArm->bInheritPitch = true;
//		SpringArm->bInheritRoll = true;
//		SpringArm->bInheritYaw = true;
//		SpringArm->bDoCollisionTest = true;
//		bUseControllerRotationYaw = false;
//		GetCharacterMovement()->bOrientRotationToMovement = true;
//		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
//
//	}
//}

void AMyCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->GetRelativeRotation() = FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed);
		break;
	}


	switch (CurrentControlMode)
	{
		case EControlMode::DIABLO:
			if (DirectionToMove.SizeSquared() > 0.0f)
			{
				GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
				AddMovementInput(DirectionToMove);

			}
			break;
	}

	auto MyAnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != MyAnimInstance)
	{
		MyAnimInstance->SetPawnSpeed(GetVelocity().Size());
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ViewChange", IE_Pressed, this, &AMyCharacter::ViewChange);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	InputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);


	InputComponent->BindAxis(TEXT("UpDown"), this, & AMyCharacter::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, & AMyCharacter::LeftRight);
	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
	InputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);
	
	InputComponent->BindAction("ToggleBuildMode", IE_Pressed, BuildManager, &UBuildManagerComponent::ToggleBuildMode);
	InputComponent->BindAction("RequestBuild", IE_Pressed, BuildManager, &UBuildManagerComponent::RequestBuild);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	//AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);


	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);


	});

	//auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if (FinalDamage > 0.0f)
	//{
	//	MyAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//}
	
	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

bool AMyCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon)
{
	FName WeaponSocket(TEXT("ik_hand_sword"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AMyCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AMyCharacter::Attack()
{
	//if (IsAttacking) return;

	////auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	////if (nullptr == AnimInstance) return;

	//MyAnim->PlayAttackMontage();
	//IsAttacking = true;

	


	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;

	}

}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//IsAttacking = false;
	IsAttacking = false;
	AttackEndComboState();
}

void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);

}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);


	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);

		}
	}
}


void AMyCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
			AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
			break;
		case EControlMode::DIABLO:
			DirectionToMove.X = NewAxisValue;
			break;
	}

}

void AMyCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void AMyCharacter::LookUp(float NewAxisValue) 
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
			AddControllerPitchInput(NewAxisValue);
			break;

	}

}

void AMyCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;

	}

}



















//void AMyCharacter::UpDown(float NewAxisValue)
//{
//	if (NewAxisValue == 0.f)
//		return;
//
//	AddMovementInput(GetActorForwardVector(), NewAxisValue);
//}
//
//void AMyCharacter::LeftRight(float NewAxisValue)
//{
//	if (NewAxisValue == 0.f)
//		return;
//
//	AddMovementInput(GetActorRightVector(), NewAxisValue);
//}
//
//void AMyCharacter::LookUp(float NewAxisValue)
//{
//	if (NewAxisValue != 0.0)
//	{
//		AddControllerPitchInput(-NewAxisValue);
//
//	}
//}
//
//void AMyCharacter::Turn(float NewAxisValue)
//{
//	if (NewAxisValue != 0.0)
//	{
//		AddControllerYawInput(NewAxisValue);
//
//	}
//}

