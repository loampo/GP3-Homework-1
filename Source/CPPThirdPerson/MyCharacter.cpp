// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "CPPThirdPersonGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	

	//Gestione armi
	WeaponInventory.SetNum(2); // Inizializza l'array con due elementi per avere massimo 2 armi alla volta
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	CurrentHealth = MaxHealth;
	CurrentMag = MagCapacity; // Caricatore pieno all'inizio
	CurrentAmmo = 20;  // Munizioni rimasti

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	//Gestione armi
	/*
	if (Gun == nullptr) return;
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this); */

	if (GunClass && WeaponInventory.Num() > 0)
	{
		AGun* StarterGun = GetWorld()->SpawnActor<AGun>(GunClass);
		AddToInventory(StarterGun);
	}

	// Mappatura nuovo input
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			LocalPlayerSubsystem->AddMappingContext(InputContext, 0);
		}
	}

	DetermineIfPlayer();

	// UI
	if (AmmoUI_WBP && bIsPlayer)
	{
		AmmoUI = CreateWidget<UUserWidget>(GetWorld(), AmmoUI_WBP);
		if (AmmoUI)
		{
			AmmoUI->AddToViewport();
		}
	UpdateAmmoUI();
	UpdateWeaponDamegeUI();
	} 
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Verifica che l'input component sia di tipo EnhancedInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind delle azioni di input
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMyCharacter::ShootInput);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AMyCharacter::ReloadInput);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &AMyCharacter::AttemptPickupWeapon);
		EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Started, this, &AMyCharacter::SwitchWeapon);
	}
}


void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * RotationSpeed * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(LookAxisVector.Y * RotationSpeed * GetWorld()->GetDeltaSeconds());
	}
}

void AMyCharacter::ShootInput(const FInputActionValue& Value)
{
	if (bIsDisabled) return;
	if (Gun != nullptr && CurrentMag > 0)
	{
		Shoot(); // Chiama la versione base
		UpdateAmmoUI();
		UE_LOG(LogTemp, Warning, TEXT("Munizioni: %d / %d"), CurrentMag, CurrentAmmo);
	}
}


void AMyCharacter::Shoot()
{
	AGun* CurrentGun = WeaponInventory.IsValidIndex(CurrentWeaponIndex) ? WeaponInventory[CurrentWeaponIndex] : nullptr;
    
	if (CurrentGun && CurrentMag > 0 && !bIsReloading)
	{
		CurrentGun->PullTrigger();
		CurrentMag--;
	}
}

void AMyCharacter::ShootAI()
{
	if (bIsDisabled) return;
	if (Gun != nullptr && CurrentMag > 0)
	{
		Shoot();
	} else if (Gun != nullptr && CurrentMag <= 0)
	{
		ReloadAIInput();
	}
}

void AMyCharacter::DetermineIfPlayer()
{
	if (GetController() && GetController()->IsPlayerController())
	{
		bIsPlayer = true;
	}
	else
	{
		bIsPlayer = false;
	}
}

bool AMyCharacter::isPlayer() const // Modo stupido per sapere se è un giocatore o un AI
{
	return bIsPlayer;
}

bool AMyCharacter::isFullAmmo()
{
	if (CurrentAmmo == MaxAmmo)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AMyCharacter::ReloadAIInput()
{
	if (!bIsReloading && Reload()) // Avvia solo se non sta già ricaricando
	{
		// Disabilita input di sparo durante la ricarica
		bIsReloading = true;
        
		// Avvia timer
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimerHandle, 
			this, 
			&AMyCharacter::FinishReloadAI, 
			ReloadTime
		);
		UE_LOG(LogTemp, Warning, TEXT("Ricarica AI iniziata!"));
	}
}

void AMyCharacter::ReloadInput(const FInputActionValue& Value)
{
	if (!bIsReloading && Reload()) // Avvia solo se non sta già ricaricando
	{
		// Disabilita input di sparo durante la ricarica
		bIsReloading = true;
        
		// Avvia timer
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimerHandle, 
			this, 
			&AMyCharacter::FinishReload, 
			ReloadTime
		);			
		UE_LOG(LogTemp, Warning, TEXT("Ricarica iniziata!"));
	}
}

bool AMyCharacter::Reload()
{
	if (CurrentAmmo <= 0 || CurrentMag == MagCapacity || bIsReloading) 
		return false;

	return true; // Solo controllo, la logica vera è in FinishReload()
}

void AMyCharacter::FinishReload()
{
	int32 Needed = MagCapacity - CurrentMag;
	int32 ReloadAmount = FMath::Min(Needed, CurrentAmmo);
    
	CurrentMag += ReloadAmount;
	CurrentAmmo -= ReloadAmount;
	UpdateAmmoUI();
    
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Ricarica completata! Munizioni: %d/%d"), CurrentMag, CurrentAmmo);
}

// Ricarica senza consumare munizioni voluto
void AMyCharacter::FinishReloadAI()
{
	int32 Needed = MagCapacity - CurrentMag;
	int32 ReloadAmount = FMath::Min(Needed, CurrentAmmo);
    
	CurrentMag += ReloadAmount;
	CurrentAmmo = MaxAmmo;
    
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Ricarica completata! Munizioni: %d/%d"), CurrentMag, CurrentAmmo);
}



float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(CurrentHealth, DamageToApply);
	CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0 && !bIsDisabled)
	{
		ACPPThirdPersonGameMode* GameMode = GetWorld()->GetAuthGameMode<ACPPThirdPersonGameMode>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		if (bIsPlayer && !bIsDisabled)
		{
			// Player finta morte
			if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				PC->DisableInput(PC);
			}
			DisableCharacter();

			// Set timer per respawn
			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				RespawnTimerHandle,
				this,
				&AMyCharacter::RespawnAtOrigin,
				5.0f,
				false
			);
		}
		else if (!bIsPlayer)
		{
			// AI 
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			GetCharacterMovement()->DisableMovement();
			DisableCharacter();
			
			FTimerHandle DestroyTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				DestroyTimerHandle,
				[this]()
				{
					Destroy();
				},
				1.0f,
				false
			);
		}
	}
		return DamageToApply;
}

bool AMyCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

bool AMyCharacter::IsPlayerDead() const
{
	if (bIsPlayer)
	{
		return IsDead();
	} else return false;
}

//Gestione Armi

void AMyCharacter::AddToInventory(AGun* NewWeapon)
{
    if (!NewWeapon) return;

    // Controlla se l'inventario è pieno
    if (WeaponInventory[0] && WeaponInventory[1])
    {
        // Sostituisci l'arma corrente
        AGun* OldWeapon = WeaponInventory[CurrentWeaponIndex];
        OldWeapon->Destroy();
        WeaponInventory[CurrentWeaponIndex] = NewWeapon;
    	UE_LOG(LogTemp, Warning, TEXT("Arma sostituita: %s"), *NewWeapon->GetName());
    }
    else
    {
        // Trova il primo slot vuoto
        int32 EmptyIndex = WeaponInventory.Find(nullptr);
        if (EmptyIndex != INDEX_NONE)
        {
            WeaponInventory[EmptyIndex] = NewWeapon;
            CurrentWeaponIndex = EmptyIndex;
        	UE_LOG(LogTemp, Warning, TEXT("Arma aggiunta: %s"), *NewWeapon->GetName());
        }
    }

    // Setup della nuova arma
    NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
    NewWeapon->SetOwner(this);
    NewWeapon->SetActorHiddenInGame(false);
	NewWeapon->SetActorEnableCollision(false);

    // Nascondi le altre armi
    for (int32 i = 0; i < WeaponInventory.Num(); i++)
    {
        if (WeaponInventory[i] && i != CurrentWeaponIndex)
        {
            WeaponInventory[i]->SetActorHiddenInGame(true);
        }
    }
}

void AMyCharacter::AddAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Min(CurrentAmmo + Amount, MaxAmmo);
	UE_LOG(LogTemp, Warning, TEXT("Munizioni aggiunte: %d"), Amount);
}

void AMyCharacter::SwitchWeapon()
{
	if (WeaponInventory[0] && WeaponInventory[1])
	{
		// Nascondi arma corrente
		WeaponInventory[CurrentWeaponIndex]->SetActorHiddenInGame(true);
		WeaponInventory[CurrentWeaponIndex]->SetActorEnableCollision(false);
    
		// Cambia indice
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
    
		// Mostra nuova arma
		if (WeaponInventory[CurrentWeaponIndex])
		{
			WeaponInventory[CurrentWeaponIndex]->SetActorHiddenInGame(false);
			WeaponInventory[CurrentWeaponIndex]->SetActorEnableCollision(false);
		}
		UpdateWeaponDamegeUI();
	}
}

void AMyCharacter::AttemptPickupWeapon()
{
    if (!bIsPlayer) return;

	FVector Start = GetActorLocation();
	FVector End = Start;
	//DrawDebugSphere(GetWorld(), Start, PickupRange, 12, FColor::Red, false, 1.0f);
    TArray<struct FHitResult> HitResults;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(PickupRange), Params))
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Collided with: %s"), *HitActor->GetName());
			}

			AGun* PotentialWeapon = Cast<AGun>(HitActor);
			if (PotentialWeapon && !PotentialWeapon->IsOwnedBy(this))
			{
				AddToInventory(PotentialWeapon);
				UpdateWeaponDamegeUI();
			}
		}
	}
}

// UI
void AMyCharacter::UpdateAmmoUI() const
{
	if (AmmoUI)
	{
		UFunction* UpdateAmmoFunction = AmmoUI->FindFunction(FName("AmmoUIUpdate"));
		if (UpdateAmmoFunction)
		{
			struct FAmmoUIValues
			{
				int32 CurrentMag;
				int32 CurrentAmmo;
			};

			FAmmoUIValues Values;
			Values.CurrentMag = CurrentMag;
			Values.CurrentAmmo = CurrentAmmo;

			AmmoUI->ProcessEvent(UpdateAmmoFunction, &Values);
		}
	}
}

void AMyCharacter::UpdateWeaponDamegeUI() const
{
	UFunction* UpdateWeaponDamageFunction = AmmoUI->FindFunction(FName("WeaponUIDamage"));
	if (UpdateWeaponDamageFunction)
	{
		struct FWeaponUIDamageValues
		{
			int32 WeaponDamage;
		};

		FWeaponUIDamageValues Values;
		Values.WeaponDamage = WeaponInventory[CurrentWeaponIndex]->GetDamage();

		AmmoUI->ProcessEvent(UpdateWeaponDamageFunction, &Values);
	}
}

// Il restart del player non funziona come speravo
void AMyCharacter::ResetPlayerState()
{
	// Ripristina salute e munizioni
	CurrentHealth = MaxHealth;
	CurrentMag = MagCapacity;
	CurrentAmmo = MaxAmmo;
	
	// Resetta l'arma solo se necessario
	if (!Gun && GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		if (Gun)
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
			Gun->SetOwner(this);
		}
	}

	// Aggiorna la UI
	if (AmmoUI_WBP)
	{
		if (AmmoUI) AmmoUI->RemoveFromParent();
		AmmoUI = CreateWidget<UUserWidget>(GetWorld(), AmmoUI_WBP);
		if (AmmoUI) AmmoUI->AddToViewport();
	}
}

// Nuova finta morte
void AMyCharacter::DisableCharacter()
{
	bIsDisabled = true;
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	if (Gun) Gun->SetActorHiddenInGame(true);
}

void AMyCharacter::EnableCharacter()
{
	bIsDisabled = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorHiddenInGame(false);
	if (Gun) Gun->SetActorHiddenInGame(false);
}

void AMyCharacter::RespawnAtOrigin()
{
	// Reset stats
	CurrentHealth = MaxHealth;
	CurrentAmmo = FMath::Min(CurrentAmmo + 20, MaxAmmo);
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->EnableInput(PC);
	}
	EnableCharacter();
}
