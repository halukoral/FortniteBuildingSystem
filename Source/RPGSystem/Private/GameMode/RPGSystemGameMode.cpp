// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RPGSystemGameMode.h"
#include "../Player/RPGSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPGSystemGameMode::ARPGSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
