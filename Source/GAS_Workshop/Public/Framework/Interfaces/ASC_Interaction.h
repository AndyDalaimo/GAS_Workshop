// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utils/GlobalUtils.h"
#include "AbilitySystemComponent.h"
#include "ASC_Interaction.generated.h"

struct FGameplayTagContainer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRemoveEffectEvent, TSubclassOf<UGameplayEffect>, EffectClass, float, LifeTime, float, Magnitude);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UASC_Interaction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAS_WORKSHOP_API IASC_Interaction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ASC_Interaction)
	UAbilitySystemComponent* GetActorAbilitySystemComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ASC_Interaction)
	FGameplayTagContainer GetActorRestrictedTags();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ASC_Interaction)
	void ReverseMovementEffectOnActor(EMovementAttributes MovementAttribute, float NewValue);
};
