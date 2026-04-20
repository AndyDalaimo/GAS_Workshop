// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Attributes/MovementAttributeSet.h"
#include "Framework/Interfaces/ASC_Interaction.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GAS_WORKSHOP_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IASC_Interaction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UWorkshopAbilitySystemComponent> WorkshopAbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UHealthAttributeSet> HealthSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UMovementAttributeSet> MovementSet;

	// Movement Attribute Changes reflected on Character custom properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementProperties")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementProperties")
	float SprintSpeed;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Movement input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	/** Look input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	/** Jump input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	/** Sprint input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RestrictedTags)
	FGameplayTagContainer RestrictedHealthTags;

	/** Handles movement input */
	void Move(const FInputActionValue& Value);

	/** Handles look input */
	void Look(const FInputActionValue& Value);

	/** Handle Sprint Action (Tied to MovementAttributeSet) */
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Sprint(const FInputActionValue& Value);
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void StopSprinting(const FInputActionValue& Value);

	// Character Movement Attribute Changes
	void OnMovementSpeedAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	void OnStaminaAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnSprintSpeedAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnJumpPowerAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;

	void SetupMovementAttributeChanges();

	UFUNCTION(BlueprintPure)
	float GetMovementSpeedAttribute() const;

	UFUNCTION(BlueprintPure)
	float GetStaminaAttribute() const;

	UFUNCTION(BlueprintPure)
	float GetSprintSpeedAttribute() const;

	UFUNCTION(BlueprintPure)
	float GetJumpPowerAttribute() const;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Get ASC from Player
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ---------------------- Overridden from ASC_Interaction -------------------------

	// Get ASC from Base Actor class with ASC_Interaction Interface (BP Native Event)
	virtual UAbilitySystemComponent* GetActorAbilitySystemComponent_Implementation() override;

	virtual FGameplayTagContainer GetActorRestrictedTags_Implementation() override;

	virtual void ReverseMovementEffectOnActor_Implementation(EMovementAttributes MovementAttribute, float NewValue) override;

};
