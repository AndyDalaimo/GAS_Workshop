// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/GlobalUtils.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Framework/Interfaces/ASC_Interaction.h"
#include "GameplayAbilityPickup.generated.h"

UCLASS()
class GAS_WORKSHOP_API AGameplayAbilityPickup : public AActor, public IASC_Interaction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowAbstract = "true"))
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowAbstract = "true"))
	UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Pickup Properties")
	FGameplayTagContainer IgnoredTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Pickup Properties")
	TSubclassOf<class UGameplayAbility> AbilityToGrantActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Pickup Properties")
	EWorkshopAbilitySlotsEnum InputIDOverride;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Pickup Properties")
	bool bClearAbility = true;

	
public:	
	// Sets default values for this actor's properties
	AGameplayAbilityPickup();

	// Check if ASC Actor can have Ability Granted / Overridden 
	UFUNCTION(BlueprintCallable)
	virtual bool CanGrantAbilityToActor(AActor* Actor) const;

	UFUNCTION()
	void OverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -------------------------------------------
	// - Helper Functions for Actor Interactions -
	// -------------------------------------------

	virtual void GrantAbilityTo(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnOverlapped"))
	void K2_OnOverlapped();

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnExited"))
	void K2_OnExited();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
