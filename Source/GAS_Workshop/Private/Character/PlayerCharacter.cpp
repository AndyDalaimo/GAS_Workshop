// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "Framework/WorkshopAbilitySystemComponent.h"
#include "Attributes/HealthAttributeSet.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WorkshopAbilitySystemComp = CreateDefaultSubobject<UWorkshopAbilitySystemComponent>(TEXT("ASC"));
	HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthSet"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WorkshopAbilitySystemComp)
	{
		WorkshopAbilitySystemComp->InitAbilityActorInfo(this, this);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return WorkshopAbilitySystemComp;
}

