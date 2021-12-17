// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectS2GameMode.h"
#include "ProjectS2Character.h"
#include "UObject/ConstructorHelpers.h"

AProjectS2GameMode::AProjectS2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
