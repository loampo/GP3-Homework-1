// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPThirdPersonGameMode.h"
#include "TangoHuntGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API ATangoHuntGameMode : public ACPPThirdPersonGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
