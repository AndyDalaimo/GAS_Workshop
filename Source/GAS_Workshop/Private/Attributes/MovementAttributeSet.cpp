// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/MovementAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UMovementAttributeSet::UMovementAttributeSet()
{
	InitMovementSpeed(600.f); // Current Character Movement Comp Max Walk Speed
	InitDefaultMovementSpeed(600.f);
	InitMaxMovementSpeed(1200.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
	InitSprintSpeed(900.f); // This speed will add to Current Movement Speed
	InitJumpPower(600.f); // Default Character Movement Comp Jump Z Velocity

	FGameplayTag FullStamTag = FGameplayTag::RequestGameplayTag(FName("Movement.Conditions.StaminaRegen"));
	StaminaRegenTag.AddTag(FullStamTag);
}


// Rules defined for attibutes before they are changed / replicated. i.e. Clamp Health value between min-max.
void UMovementAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange: Movement Attribute '%s'"), *Attribute.AttributeName);

	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 100.f, GetMaxMovementSpeed()); // Clamp to min-max walk speed
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina()); // Clamp Stamina between Stamina and Character's Max
	}

	Super::PreAttributeChange(Attribute, NewValue);
}

// Broadcast delegates to appropriate classes that need these attribute values
void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);

	if (Attribute == GetMovementSpeedAttribute())
	{
		OnMovementSpeedChanged.Broadcast(this, OldValue, NewValue);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		OnStaminaChanged.Broadcast(this, OldValue, NewValue);
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		const float CurrentStamina = GetStamina();
		OnStaminaChanged.Broadcast(this, CurrentStamina, CurrentStamina);
	}
	else if (Attribute == GetSprintSpeedAttribute())
	{
		OnSprintSpeedChanged.Broadcast(this, OldValue, NewValue);
	}
	else if (Attribute == GetJumpPowerAttribute())
	{
		OnJumpPowerChanged.Broadcast(this, OldValue, NewValue);
	}
}

// Attributes effected by meta attribute MovementSlow
void UMovementAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UE_LOG(LogTemp, Warning, TEXT("PostApply: Gameplay Effect '%s' effect"), *Data.EffectSpec.Def->GetClass()->GetName());

	if (Data.EvaluatedData.Attribute == GetMovementSlowAttribute())
	{
		// Convert into -Speed and -JumpPower and then clamp
		const float SlowValue = GetMovementSlow();
		const float OldMovementSpeedValue = GetMovementSpeed();
		const float NewMovementSpeedValue = FMath::Clamp(OldMovementSpeedValue - SlowValue, 100.f, GetMaxMovementSpeed());

		if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
		{
			if (OldMovementSpeedValue != NewMovementSpeedValue)
			{
				// Set New Max Walk Speed on Character 
				SetMovementSpeed(NewMovementSpeedValue);
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetStaminaDrainAttribute())
	{
		const float DrainValue = GetStaminaDrain();
		const float OldStaminaValue = GetStamina();
		const float NewStaminaValue = FMath::Clamp(OldStaminaValue - DrainValue, 0.f, GetMaxStamina());

		if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
		{
			if (OldStaminaValue != NewStaminaValue)
			{
				// Set New Samina on Character
				SetStamina(NewStaminaValue);
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		if (GetStamina() >= GetMaxStamina())
		{
			/*if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
			{
				FGameplayTag FullStamTag = FGameplayTag::RequestGameplayTag(FName("Movement.Conditions.FullStamina"));
				if (!OwningAbilitySystemComponent->HasMatchingGameplayTag(FullStamTag))
				{
					OwningAbilitySystemComponent->AddLooseGameplayTag(FullStamTag);
				}
			}*/

			if (UAbilitySystemComponent* OwningAbilitySystemComponent = GetValid(GetOwningAbilitySystemComponent()))
			{
				OwningAbilitySystemComponent->RemoveActiveEffectsWithAppliedTags(StaminaRegenTag);
			}
		}
	}
}

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMovementAttributeSet, MovementSpeed);
	DOREPLIFETIME(UMovementAttributeSet, Stamina);
	DOREPLIFETIME(UMovementAttributeSet, MaxStamina);
	DOREPLIFETIME(UMovementAttributeSet, SprintSpeed);
	DOREPLIFETIME(UMovementAttributeSet, JumpPower);
}

 

void UMovementAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MovementSpeed, OldValue);

	const float OldMovementSpeed = OldValue.GetCurrentValue();
	const float NewMovementSpeed = GetMovementSpeed();
	OnMovementSpeedChanged.Broadcast(this, OldMovementSpeed, NewMovementSpeed);
}

void UMovementAttributeSet::OnRep_DefaultMovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MaxMovementSpeed, OldValue);

	const float CurrentMovementSpeed = GetMovementSpeed();
	OnDefaultMovementSpeedChanged.Broadcast(this, CurrentMovementSpeed, CurrentMovementSpeed);
}

void UMovementAttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MaxMovementSpeed, OldValue);

	const float CurrentMovementSpeed = GetMovementSpeed();
	OnMaxMovementSpeedChanged.Broadcast(this, CurrentMovementSpeed, CurrentMovementSpeed);
}

void UMovementAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, Stamina, OldValue);

	const float OldStamina = OldValue.GetCurrentValue();
	const float NewStamina = GetStamina();
	OnStaminaChanged.Broadcast(this, OldStamina, NewStamina);
}

void UMovementAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MaxStamina, OldValue);

	const float CurrentStamina = GetStamina();
	OnStaminaChanged.Broadcast(this, CurrentStamina, CurrentStamina);
}

void UMovementAttributeSet::OnRep_SprintSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, SprintSpeed, OldValue);

	const float OldSprintSpeed = OldValue.GetCurrentValue();
	const float NewSprintSpeed = GetSprintSpeed();
	OnSprintSpeedChanged.Broadcast(this, OldSprintSpeed, NewSprintSpeed);
}

void UMovementAttributeSet::OnRep_JumpPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, JumpPower, OldValue);

	const float OldJumpPower = OldValue.GetCurrentValue();
	const float NewJumpPower = GetJumpPower();
	OnJumpPowerChanged.Broadcast(this, OldJumpPower, NewJumpPower);

}
