// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Add item to existing InventoryItems. If ItemID exists, add to existing quantity. 
void UInventoryComponent::AddItem(FInventoryItem newitem)
{
	for (FInventoryItem item : InventoryItems)
	{
		if (item.ItemID == newitem.ItemID) // Item exists in Inventory
		{
			item.Quantity += newitem.Quantity;
			return;
		}
	}

	InventoryItems.Push(newitem);
	SortInventory();
}

// Remove indicated quantity. If quantity is -1, remove entire stack.
void UInventoryComponent::RemoveItem(int ItemID, int quantityToRemove)
{

	for (FInventoryItem item : InventoryItems)
	{
		if (item.ItemID == ItemID)
		{
			if (quantityToRemove == -1)
			{
				InventoryItems.Remove(item);
			}
			else {
				item.Quantity -= quantityToRemove;
			}
		}
	}
}


// TODO -> Sort Items on things like Category, Quantity, Cost, etc.
void UInventoryComponent::SortInventory()
{
}