// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASP6GameMode.h"
#include "GASP6Character.h"
#include "UObject/ConstructorHelpers.h"

AGASP6GameMode::AGASP6GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
