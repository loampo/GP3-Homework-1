// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPThirdPersonGameMode.h"
#include "KingOfTheHillGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CPPTHIRDPERSON_API AKingOfTheHillGameMode : public ACPPThirdPersonGameMode
{
	GENERATED_BODY()
	
public:
	AKingOfTheHillGameMode();
	
	virtual void PawnKilled(APawn* PawnKilled) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AMyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval2 = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnAILocation = FVector(2400, 1212, 100);
	FVector SpawnAILocation2 = FVector(500, 1700, 100);

	FTimerHandle SpawnTimerHandle;
	FTimerHandle SpawnTimerHandle2;

	UFUNCTION()
	void SpawnEnemyWave();
	UFUNCTION()
	void SpawnEnemyWave2();
	void UpdateScoreUI() const;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ScoreUI_WBP;

	UPROPERTY()
	UUserWidget* ScoreUI;
	UPROPERTY(EditAnywhere, Category= "UI")
	TSubclassOf<UUserWidget> WinUI_WBP;
	UPROPERTY()
	UUserWidget* WinUI;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoseUI_WBP;
	UPROPERTY()
	UUserWidget* LoseUI;
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
	int32 ScoreToWin = 100;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
	float ScoreUpdateInterval = 1.0f;

private:
	FTimerHandle ScoreTimerHandle;
	class ACapturePoint* CapturePoint;

	UPROPERTY()
	int32 PlayerScore = 0;

	UPROPERTY()
	int32 AIScore = 0;
	

	void UpdateScores();
	void CheckWinCondition();
	void RestartLevel();

	FTimerHandle RespawnTimerHandle;
};
