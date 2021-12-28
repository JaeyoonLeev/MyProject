// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "UObject/ConstructorHelpers.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/Book/GameData/MyCharacterData.MyCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MyCharacter(*CharacterDataPath);
	//MYCHECK(DT_MyChacter.Succeeded())
	if (DT_MyCharacter.Succeeded())
	{
		MyCharacterTable = DT_MyCharacter.Object;
	}
}


void UMyGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Drop of Level 20 MyCharacter : %d"), GetMyCharacterData(20)->DropExp);
}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));

}