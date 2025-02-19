// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPThirdPersonGameMode.h"
#include "CPPThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPPThirdPersonGameMode::ACPPThirdPersonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}

void ACPPThirdPersonGameMode::PawnKilled(APawn* PawnKilled)
{
}
