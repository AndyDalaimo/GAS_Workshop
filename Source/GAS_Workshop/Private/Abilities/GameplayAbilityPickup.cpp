// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbilityPickup.h"

#include "AbilitySystemComponent.h"

// Sets default values
AGameplayAbilityPickup::AGameplayAbilityPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(FName("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);

	IgnoredTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));

}


// Called when the game starts or when spawned
void AGameplayAbilityPickup::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGameplayAbilityPickup::OverlapStarted);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AGameplayAbilityPickup::OverlapEnded);
	
}


// Called every frame
void AGameplayAbilityPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Only grant abilities to Actors with Ability System Components
bool AGameplayAbilityPickup::CanGrantAbilityToActor(AActor* Actor) const
{
	if (Actor->Implements<UASC_Interaction>())
	{
		UAbilitySystemComponent* ActorASC = IASC_Interaction::Execute_GetActorAbilitySystemComponent(Actor);
		if (IsValid(ActorASC))
		{
			return Actor && IsValid(this) && !ActorASC->HasAnyMatchingGameplayTags(IgnoredTags);
		}
	}
	return false;
}

void AGameplayAbilityPickup::OverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanGrantAbilityToActor(OtherActor))
	{
		ActorToGrantAbilityTo = OtherActor;
		// GrantAbilityTo(OtherActor);

		// Trigger Gameplay Cues, etc. 
		K2_OnOverlapped();
	}
}

void AGameplayAbilityPickup::OverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorToGrantAbilityTo = nullptr;
	// Trigger Gameplay Cues, etc. 
	K2_OnExited();

}

// Clear the Ability in the Input 
void AGameplayAbilityPickup::GrantAbilityTo(AActor* Actor)
{
	if (!AbilityToGrantActor) return;

	UAbilitySystemComponent* ActorASC = IASC_Interaction::Execute_GetActorAbilitySystemComponent(Actor);
	if (bClearAbility)
	{
		ActorASC->ClearAllAbilitiesWithInputID(static_cast<int>(InputIDOverride));
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityToGrantActor, 1, static_cast<int>(InputIDOverride));
		ActorASC->GiveAbility(AbilitySpec);
	}
}