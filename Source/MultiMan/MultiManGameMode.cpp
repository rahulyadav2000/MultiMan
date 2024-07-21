// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiManGameMode.h"
#include "MultiManCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiManGameMode::AMultiManGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

