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
void UInventoryComponent::AddItem(FInventoryItem newItem)
{
	int it = 0;
	for (FInventoryItem item : InventoryItems)
	{
		if (item.ItemID == newItem.ItemID) // Item exists in Inventory
		{
			InventoryItems[it].Quantity += newItem.Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Added Item ID #: %d New Quantity: %d"), newItem.ItemID, InventoryItems[it].Quantity);
			return;
		}
		it++;
	}
	InventoryItems.Push(newItem);
	UE_LOG(LogTemp, Warning, TEXT("Added %d amount of Item to Inventory. ID #: %d"), newItem.Quantity, newItem.ItemID);
	SortInventory();
}

// Remove indicated quantity. If quantity is -1, remove entire stack.
void UInventoryComponent::RemoveItem(int ID, int quantityToRemove)
{
	int it = 0;

	for (FInventoryItem item : InventoryItems)
	{
		if (item.ItemID == ID)
		{
			if (quantityToRemove == -1)
			{
				InventoryItems.RemoveAt(it, EAllowShrinking::Yes);
			}
			else {
				item.Quantity -= quantityToRemove;
			}
		}
		it++;
	}
}



// TODO -> Sort Items on things like Category, Quantity, Cost, etc.
void UInventoryComponent::SortInventory()
{
}

// 
void UInventoryComponent::AddPickupToInventory_Implementation(FInventoryItem ItemData)
{
	AddItem(ItemData);
}