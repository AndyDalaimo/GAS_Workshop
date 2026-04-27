// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/WorkshopAbilitySystemComponent.h"
#include "Attributes/HealthAttributeSet.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ASC and attach a default Attribute Set (Health)
	WorkshopAbilitySystemComp = CreateDefaultSubobject<UWorkshopAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthSet"));
	MovementSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("MovementSet"));


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // Camera distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	RestrictedHealthTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize AbiltiySystemComp
	if (WorkshopAbilitySystemComp)
	{
		WorkshopAbilitySystemComp->InitAbilityActorInfo(this, this);

		if (HasAuthority())
		{
			SetupMovementAttributeChanges();
		}
	}

	// Add Enhanced Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
		}
	}

	const FTopLevelAssetPath EnumName("/Script/GAS_Workshop.EWorkshopAbilitySlotsEnum");
	FGameplayAbilityInputBinds Binds("IA_ConfirmTargeting", "IA_CancelTargeting", EnumName);
	WorkshopAbilitySystemComp->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// ----------------- MOVEMENT --------------------

void APlayerCharacter::StopStaminaRegen_Implementation()
{
}

void APlayerCharacter::Sprint_Implementation(const FInputActionValue& Value)
{
}

void APlayerCharacter::StopSprinting_Implementation(const FInputActionValue& Value)
{
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return WorkshopAbilitySystemComp;
}

UAbilitySystemComponent* APlayerCharacter::GetActorAbilitySystemComponent_Implementation()
{
	return WorkshopAbilitySystemComp;
}

FGameplayTagContainer APlayerCharacter::GetActorRestrictedTags_Implementation()
{
	return RestrictedHealthTags;
}

// If Character has passed through a GameplayEffectTriggerVolume and the Effect is to be remvoed, this will 
// get the Value/Attribute modified and call the appropriate function to reset Attribute to previous state
void APlayerCharacter::ReverseMovementEffectOnActor_Implementation(EMovementAttributes MovementAttribute, float NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Attribute Modify Value:  '%.2f'"), NewValue); 
	switch (MovementAttribute)
	{
		case EMovementAttributes::MA_MovementSpeed:
			MovementSet->SetMovementSpeed(MovementSet->GetMovementSpeed() + NewValue);
			MovementSet->SetMovementSlow(MovementSet->GetMovementSlow() - NewValue);
			break;
		case EMovementAttributes::MA_Stamina:
			MovementSet->SetStamina(MovementSet->GetStamina() + NewValue);
			break;
		case EMovementAttributes::MA_SprintSpeed:
			MovementSet->SetSprintSpeed(MovementSet->GetSprintSpeed() - NewValue);
			break;
		case EMovementAttributes::MA_JumpPower:
			MovementSet->SetJumpPower(MovementSet->GetJumpPower() - NewValue);
			break;
	}
}




// ---------------------------------------------------------------------------------------------------
// ------------------------------- On Player's Attributes Changed ------------------------------------
// ---------------------------------------------------------------------------------------------------

void APlayerCharacter::SetupMovementAttributeChanges()
{
	WorkshopAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetMovementSpeedAttribute()).AddUObject(this, &APlayerCharacter::OnMovementSpeedAttributeChanged);
	WorkshopAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetStaminaAttribute()).AddUObject(this, &APlayerCharacter::OnStaminaAttributeChanged);
	WorkshopAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetSprintSpeedAttribute()).AddUObject(this, &APlayerCharacter::OnSprintSpeedAttributeChanged);
	WorkshopAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetJumpPowerAttribute()).AddUObject(this, &APlayerCharacter::OnJumpPowerAttributeChanged);
}




void APlayerCharacter::OnMovementSpeedAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeedAttribute();
}

void APlayerCharacter::OnStaminaAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) 
{
	Stamina = GetStaminaAttribute();
}

void APlayerCharacter::OnSprintSpeedAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) 
{
	SprintSpeed = GetSprintSpeedAttribute();
}

void APlayerCharacter::OnJumpPowerAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetCharacterMovement()->JumpZVelocity = GetJumpPowerAttribute();
}



float APlayerCharacter::GetMovementSpeedAttribute() const
{
	if (IsValid(MovementSet) == false)
	{
		return 0.0f;
	}

	return MovementSet->GetMovementSpeed();
}

float APlayerCharacter::GetStaminaAttribute() const
{
	if (IsValid(MovementSet) == false)
	{
		return 0.0f;
	}

	return MovementSet->GetStamina();
}

float APlayerCharacter::GetSprintSpeedAttribute() const
{
	if (IsValid(MovementSet) == false)
	{
		return 0.0f;
	}

	return MovementSet->GetSprintSpeed();
}

float APlayerCharacter::GetJumpPowerAttribute() const
{
	if (IsValid(MovementSet) == false)
	{
		return 0.0f;
	}

	return MovementSet->GetJumpPower();
}
