// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> LoseScreenClass;

	float RestartDelay = 5.f;

	FTimerHandle RestartTimer;

public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
};
