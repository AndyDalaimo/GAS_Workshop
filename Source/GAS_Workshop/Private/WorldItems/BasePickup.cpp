// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItems/BasePickup.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;

	PickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollision"));
	PickupCollision->SetupAttachment(PickupMesh);

}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();

	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OverlapStarted);
	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &ABasePickup::OverlapEnded);
	
}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePickup::OverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UActorComponent* OC = OtherActor->GetComponentsByInterface(UInventoryInterface::StaticClass())[0];

	if (IsValid(OC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PICKING UP ITEM FOR INVENTORY"));
		IInventoryInterface::Execute_AddPickupToInventory(OC, ItemData);
	}
}

void ABasePickup::OverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

