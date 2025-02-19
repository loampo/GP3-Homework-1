// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPPThirdPersonGameMode.generated.h"

UCLASS(minimalapi)
class ACPPThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACPPThirdPersonGameMode();
	virtual void PawnKilled(APawn* PawnKilled);
};



