// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWorkshopAbilitySlotsEnum : uint8
{
	IA_PrimaryAbility,
	IA_SecondaryAbility,
};

/**
 * 
 */
class GAS_WORKSHOP_API GlobalUtils
{
public:
	GlobalUtils();
	~GlobalUtils();
};
