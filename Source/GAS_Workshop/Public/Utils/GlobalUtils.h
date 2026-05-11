// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GlobalUtils.generated.h"

UENUM(BlueprintType)
enum class EWorkshopAbilitySlotsEnum : uint8
{
	IA_PrimaryAbility,
	IA_SecondaryAbility,
	IA_Hotbar_1,
	IA_Hotbar_2,
	IA_Hotbar_3,
	IA_Hotbar_4,
};

UENUM(BlueprintType)
enum class EMovementAttributes : uint8
{
	MA_NONE,
	MA_MovementSpeed,
	MA_Stamina,
	MA_MaxStamina,
	MA_SprintSpeed,
	MA_JumpPower
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItemStruct")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItemStruct")
	int ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItemStruct")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItemStruct")
	int Quantity;

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
