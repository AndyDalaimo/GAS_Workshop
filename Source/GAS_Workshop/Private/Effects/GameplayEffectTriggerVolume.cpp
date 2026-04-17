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

	if (!ApplyToActorTags.IsEmpty()) IgnoredTags.AppendTags(ApplyToActorTags);
}



// Called every frame
void AGameplayEffectTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Only apply effects to Actors with Ability System Components
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

		ApplyEffectTo(OtherActor);
		bIsActive = false;

		// Trigger Gameplay Cues, etc. 
		K2_OnOverlapped();
	}
}

void AGameplayEffectTriggerVolume::OverlapEnded(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (EffectUpdateOnExit == ERemoveEffectOnExit::RemoveEffect && OtherActor->Implements<UASC_Interaction>()) RemoveEffectFrom(OtherActor);
	if (!bIsActive)
	{
		bIsActive = true;

		// Trigger Gameplay Cues, etc.
		K2_OnExited();
	}
}

// ------------------------------------------------------------------------
// ------- Event Functions to call and Apply/Remove Gameplay Effects ------
// ------------------------------------------------------------------------

// If Actor can be effected (Does not have Ignore Tags) Apply effects to their according ASC
void AGameplayEffectTriggerVolume::ApplyEffectTo(AActor* Actor)
{
	UAbilitySystemComponent* ASC = IASC_Interaction::Execute_GetActorAbilitySystemComponent(Actor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Actor's ASC is null."), *FString(Actor->GetName()));
		return;
	}

	// Create Gameplay Effect Context to add to Actor
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Add Gameplay Effects to ASC of interacting Actor
	for (TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
	{
		if (!EffectClass) continue;

		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(EffectClass, 0.f, EffectContext);

		ASC->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());

	}

	// Apply these tags to Actor if specified
	if (!ApplyToActorTags.IsEmpty())
	{
		ASC->AddLooseGameplayTags(ApplyToActorTags);
	}
}

// Remove Effect from Actor IF this Volume is tagged with RemoveOnExit::RemoveEffect
void AGameplayEffectTriggerVolume::RemoveEffectFrom(AActor* Actor)
{
	UAbilitySystemComponent* ASC = IASC_Interaction::Execute_GetActorAbilitySystemComponent(Actor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Actor's ASC is null."), *FString(Actor->GetName()));
		return;
	}

	for (TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
	{
		if (!EffectClass) continue;
		UE_LOG(LogTemp, Warning, TEXT("%s Effect Removed"), *FString(EffectClass->GetName()));
		ASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, nullptr, 1);
	}

	// Remove these tags to Actor if specified
	if (!ApplyToActorTags.IsEmpty())
	{
		ASC->RemoveLooseGameplayTags(ApplyToActorTags);
	}
	
	if (MovementAttributeToReverse != EMovementAttributes::MA_NONE)
	{
		IASC_Interaction::Execute_ReverseMovementEffectOnActor(Actor, MovementAttributeToReverse, AttributeModifyValue);
	}
}

