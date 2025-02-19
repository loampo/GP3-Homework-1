// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AGun;

UCLASS()
class CPPTHIRDPERSON_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 1;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AGun> GunClass;

	//Non riesco a respawnare quindi faccio così, dovrei rifare tutto il sistema di respawn ma richiede troppo tempo
	bool bIsDisabled = false;
	FVector SpawnLocation;
	FRotator SpawnRotation;

	void DisableCharacter();
	void EnableCharacter();
	void RespawnAtOrigin();

	UPROPERTY()
	AGun* Gun;

	// Nuovo input system
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickUpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ChangeWeaponAction;
	// Munizioni
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 MagCapacity = 10; // Capacità del caricatore
    
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 CurrentMag; // Proiettili nel caricatore corrente
    
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 MaxAmmo = 100; // Munizioni totali
    
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 CurrentAmmo; // Munizioni rimanenti

	//Check se il player è il player e non un AI Controller
	UPROPERTY(VisibleAnywhere, Category = "Controller")
	bool bIsPlayer = false;

	void DetermineIfPlayer();
	void ReloadAIInput();

	// Ricarica
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadTime = 2.0f;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool bIsReloading = false;
	UFUNCTION()
	void ReloadInput(const FInputActionValue& Value); // Per l'input
	FTimerHandle ReloadTimerHandle;
	void FinishReload();
	void FinishReloadAI();
	bool Reload(); // Restituisce true se la ricarica può iniziare

	// Gestione Armi
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TArray<AGun*> WeaponInventory;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float PickupRange = 100.f;

	UFUNCTION()
	void SwitchWeapon();

	UFUNCTION()
	void AttemptPickupWeapon();

	// UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> AmmoUI_WBP;

	UPROPERTY()
	class UUserWidget* AmmoUI;


public:
	void UpdateAmmoUI() const;
	void UpdateWeaponDamegeUI() const;
	void ResetPlayerState();

	AMyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	bool IsPlayerDead() const;

	bool isPlayer() const;

	bool isFullAmmo();

	UFUNCTION()
	void Shoot();
	UFUNCTION()
	void ShootAI(); // Per l'AI

	UFUNCTION()
	void ShootInput(const FInputActionValue& Value);

	//Gestione Armi
	UFUNCTION()
	void AddToInventory(AGun* NewWeapon);

	UFUNCTION()
	void AddAmmo(int32 Amount);

	
protected:

	virtual void BeginPlay() override;

};
