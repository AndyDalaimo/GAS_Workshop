// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Framework/Interfaces/ASC_Interaction.h"
#include "NPCCharacter.generated.h"

UCLASS()
class GAS_WORKSHOP_API ANPCCharacter : public ACharacter, public IAbilitySystemInterface, public IASC_Interaction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UWorkshopAbilitySystemComponent> WorkshopAbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UHealthAttributeSet> HealthSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UMovementAttributeSet> MovementSet;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RestrictedTags)
	FGameplayTagContainer RestrictedHealthTags;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Get ASC from NPC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Get ASC from Base Actor class with ASC_Interaction Interface (BP Native Event)
	virtual UAbilitySystemComponent* GetActorAbilitySystemComponent_Implementation() override;

	virtual FGameplayTagContainer GetActorRestrictedTags_Implementation() override;

};
