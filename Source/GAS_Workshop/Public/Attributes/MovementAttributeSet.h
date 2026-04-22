// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Framework/Interfaces/ASC_Interaction.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Delegate to be broadcasted after an Attribute is changed 
// NOTE ** -> Make sure to change Delegate name to more appropriate Attribute name prefix
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementAttributeChangedEvent, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

/**
 * 
 */
UCLASS()
class GAS_WORKSHOP_API UMovementAttributeSet : public UAttributeSet, public IASC_Interaction
{
	GENERATED_BODY()
	

	UMovementAttributeSet();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, meta = (HideFromModifiers))
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSpeed);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DefaultMovementSpeed, meta = (HideFromModifiers))
	FGameplayAttributeData DefaultMovementSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, DefaultMovementSpeed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxMovementSpeed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, Stamina);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxStamina);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SprintSpeed)
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, SprintSpeed);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_JumpPower)
	FGameplayAttributeData JumpPower;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, JumpPower);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_DefaultMovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SprintSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_JumpPower(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnMovementSpeedChanged;
	
	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnDefaultMovementSpeedChanged;
	
	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnMaxMovementSpeedChanged;

	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnSprintSpeedChanged;

	UPROPERTY(BlueprintAssignable)
	FMovementAttributeChangedEvent OnJumpPowerChanged;

	// Slow Value calculated during GE. Meta Attribute (Will Effect Speed and JumpPower)
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MovementSlow;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSlow);
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData StaminaDrain;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, StaminaDrain);

protected:
	FGameplayTagContainer StaminaRegenTag;
};
