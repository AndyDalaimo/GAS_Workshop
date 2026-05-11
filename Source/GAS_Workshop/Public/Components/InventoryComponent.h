// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/GlobalUtils.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_WORKSHOP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryItems")
	TArray<FInventoryItem> InventoryItems;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Internal use.
	// TODO -> Sort Items on things like Category, Quantity, Cost, etc.
	void SortInventory();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Add item to existing InventoryItems. If ItemID exists, add to existing quantity. 
	UFUNCTION(BlueprintCallable)
	void AddItem(FInventoryItem newitem);

	// Remove indicated quantity. If quantity is -1, remove entire stack.
	UFUNCTION(BlueprintCallable)
	void RemoveItem(int ItemID, int quantityToRemove = -1);
		
};
