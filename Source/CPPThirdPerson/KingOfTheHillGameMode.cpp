// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePoint.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "KingOfTheHillGameMode.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

AKingOfTheHillGameMode::AKingOfTheHillGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKingOfTheHillGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	// Solo per AI
	if (!Cast<APlayerController>(PawnKilled->GetController()))
	{
		return;
	}
}


void AKingOfTheHillGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Trova il CapturePoint nella mappa (anche se ne ho messo solo 1)
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturePoint::StaticClass(), FoundActors);
	if(FoundActors.Num() > 0)
	{
		CapturePoint = Cast<ACapturePoint>(FoundActors[0]);
	}
	
	if (ScoreUI_WBP != nullptr)
	{
		ScoreUI = CreateWidget<UUserWidget>(GetWorld(), ScoreUI_WBP);
		if (ScoreUI != nullptr)
		{
			ScoreUI->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(ScoreTimerHandle, this, &AKingOfTheHillGameMode::UpdateScores, ScoreUpdateInterval, true);

	// Spawn Nemici
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AKingOfTheHillGameMode::SpawnEnemyWave, SpawnInterval, true);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle2, this, &AKingOfTheHillGameMode::SpawnEnemyWave2, SpawnInterval2, true);

}

void AKingOfTheHillGameMode::UpdateScores()
{

	if(!CapturePoint->IsCaptured() && !CapturePoint->IsConflict() && PlayerScore < ScoreToWin && AIScore < ScoreToWin)
	{
		AIScore++;
		UE_LOG(LogTemp, Warning, TEXT("AI Score: %d"), AIScore);
	}
	else if(CapturePoint->IsCaptured() && !CapturePoint->IsConflict() && PlayerScore < ScoreToWin && AIScore < ScoreToWin)
	{
		PlayerScore++;
		UE_LOG(LogTemp, Warning, TEXT("Player Score: %d"), PlayerScore);
	}
	UpdateScoreUI();
	CheckWinCondition();
}

void AKingOfTheHillGameMode::CheckWinCondition()
{
	if(PlayerScore >= ScoreToWin)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER WINS!"));
		if (WinUI_WBP != nullptr)
		{
			WinUI = CreateWidget<UUserWidget>(GetWorld(), WinUI_WBP);
			if (WinUI != nullptr)
			{
				WinUI->AddToViewport();
				if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PC->SetShowMouseCursor(true);
					PC->DisableInput(PC);
				}
			}
		}
		GetWorldTimerManager().ClearTimer(ScoreTimerHandle);
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		// Riavvio del livello
		GetWorldTimerManager().SetTimer(
				RespawnTimerHandle,
				this,
				&AKingOfTheHillGameMode::RestartLevel,
				5.0f,
				false
			);
	}
	else if(AIScore >= ScoreToWin)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI WINS!"));
		if (LoseUI_WBP != nullptr)
		{
			LoseUI = CreateWidget<UUserWidget>(GetWorld(), LoseUI_WBP);
			if (LoseUI != nullptr)
			{
				LoseUI->AddToViewport();
				if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PC->SetShowMouseCursor(true);
					PC->DisableInput(PC);
				}
			}
		}
		GetWorldTimerManager().ClearTimer(ScoreTimerHandle);
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        
		// Riavvio del livello
		GetWorldTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&AKingOfTheHillGameMode::RestartLevel,
			5.0f,
			false
		);
	}
}

void AKingOfTheHillGameMode::RestartLevel()
{
	// A quanto sembra aprire è meglio del riavvio
	const FString CurrentLevelName = GetWorld()->GetMapName();
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName), false);
}

void AKingOfTheHillGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKingOfTheHillGameMode::SpawnEnemyWave()
{
	if(EnemyClass && GetWorld())
	{
		AMyCharacter* NewEnemy = GetWorld()->SpawnActor<AMyCharacter>(
			EnemyClass,
			SpawnAILocation,
			FRotator::ZeroRotator
		);
		NewEnemy->SpawnDefaultController();
	}
}
void AKingOfTheHillGameMode::SpawnEnemyWave2()
{
	if(EnemyClass && GetWorld())
	{
		AMyCharacter* NewEnemy = GetWorld()->SpawnActor<AMyCharacter>(
			EnemyClass,
			SpawnAILocation2,
			FRotator::ZeroRotator
		);
		NewEnemy->SpawnDefaultController();
	}
}

void AKingOfTheHillGameMode::UpdateScoreUI() const
{
	if (ScoreUI)
	{
		UFunction* UpdateScoreFunction = ScoreUI->FindFunction(FName("UpdateScore"));
		if (UpdateScoreFunction)
		{
			struct FScoreUIValues
			{
				int32 MaxScore;
				int32 PlayerScore;
				int32 EnemyScore;
			};

			FScoreUIValues Values;
			Values.MaxScore = ScoreToWin;
			Values.PlayerScore = PlayerScore;
			Values.EnemyScore = AIScore;

			ScoreUI->ProcessEvent(UpdateScoreFunction, &Values);
		}
	}
}