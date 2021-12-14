// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Myproject.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "MyItemBox.generated.h"


UCLASS()
class MYPROJECT_API AMyItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Box)

	//trigger 사용시 class 붙여야함!
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;
	
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AMyWeapon> WeaponItemClass;

private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
