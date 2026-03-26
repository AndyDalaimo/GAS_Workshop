// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/GameplayEffectTriggerVolume.h"

#include "AbilitySystemComponent.h"


// Sets default values
AGameplayEffectTriggerVolume::AGameplayEffectTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(FName("CollisionComp"));
	RootComponent = CollisionComp;

	IgnoredTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));

}

// Called when the game starts or when spawned
void AGameplayEffectTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGameplayEffectTriggerVolume::OverlapStarted);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AGameplayEffectTriggerVolume::OverlapEnded);

}

// Called every frame
void AGameplayEffectTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGameplayEffectTriggerVolume::CanEffectCharacter(AActor* Actor) const
{
	if (Actor->Implements<UASC_Interaction>())
	{
		UAbilitySystemComponent* ActorASC = IASC_Interaction::Execute_GetActorAbilitySystemComponent(Actor);
		if (IsValid(ActorASC))
		{
			return bIsActive && Actor && IsValid(this) && !ActorASC->HasAnyMatchingGameplayTags(IgnoredTags);
		}
	}
	return false;
}

void AGameplayEffectTriggerVolume::OverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor Overlap"));
	if (CanEffectCharacter(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Actor's ASC can be effected."), *FString(OtherActor->GetName()));
	}
}

void AGameplayEffectTriggerVolume::OverlapEnded(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

