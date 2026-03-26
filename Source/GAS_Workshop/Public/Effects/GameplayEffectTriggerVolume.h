// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Interfaces/ASC_Interaction.h"
#include "GameplayEffectTriggerVolume.generated.h"

UENUM(BlueprintType)
enum class ERemoveEffectOnExit : uint8
{
	None = 0 UMETA(Hidden),
	RemoveEffect = 1,
	KeepEffect = 2
};

UCLASS()
class GAS_WORKSHOP_API AGameplayEffectTriggerVolume : public AActor, public IASC_Interaction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowAbstract = "true"))
		UBoxComponent* CollisionComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Volume Properties")
		FGameplayTagContainer IgnoredTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Volume Properties")
		TArray<TSubclassOf<class UGameplayEffect>> EffectClasses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger Volume Properties")
		bool bIsActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger Volume Properties")
		ERemoveEffectOnExit EffectUpdateOnExit = ERemoveEffectOnExit::None;
		
public:	
	// Sets default values for this actor's properties
	AGameplayEffectTriggerVolume();

	// Check if Pawn should be effected by GE Volume
	UFUNCTION(BlueprintCallable)
	virtual bool CanEffectCharacter(AActor* Actor) const;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
