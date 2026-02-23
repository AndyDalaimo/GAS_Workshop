// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/WorkshopGameMode.h"

#include "UObject/ConstructorHelpers.h"

AWorkshopGameMode::AWorkshopGameMode()
{
	// Set Default Pawn class to our Blueprint Player Character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}
