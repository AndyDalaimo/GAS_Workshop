// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPCCharacter.h"

#include "Framework/WorkshopAbilitySystemComponent.h"
#include "Attributes/HealthAttributeSet.h"
#include "Attributes/MovementAttributeSet.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WorkshopAbilitySystemComp = CreateDefaultSubobject<UWorkshopAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthSet"));
	MovementSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("MovementSet"));

	RestrictedHealthTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));

}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize AbiltiySystemComp
	if (WorkshopAbilitySystemComp)
	{
		WorkshopAbilitySystemComp->InitAbilityActorInfo(this, this);
	}
	
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ANPCCharacter::GetAbilitySystemComponent() const
{
	return WorkshopAbilitySystemComp;
}

UAbilitySystemComponent* ANPCCharacter::GetActorAbilitySystemComponent_Implementation()
{
	return WorkshopAbilitySystemComp;
}

FGameplayTagContainer ANPCCharacter::GetActorRestrictedTags_Implementation()
{
	return RestrictedHealthTags;
}

