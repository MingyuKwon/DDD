// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/DDDGameMode.h"
#include "Character/DDDCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADDDGameMode::ADDDGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
