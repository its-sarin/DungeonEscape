// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!ActorToTrigger)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorToTrigger is not set in TriggerComponent on %s"), *GetOwner()->GetName());
	}
	else
	{		
		Mover = ActorToTrigger->FindComponentByClass<UMover>();
		Faller = ActorToTrigger->FindComponentByClass<UFaller>();
		
		if (!Mover && !Faller)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Mover or Faller on ActorToTrigger in TriggerComponent on %s"), *GetOwner()->GetName());
		}
	}

	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);

		if (TriggerTag.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerTag is not set in TriggerComponent on %s"), *GetOwner()->GetName());
		}
	}	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
	else if (Faller)
	{
		Faller->SetShouldFall(IsTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot trigger mover because Mover or Faller is null in TriggerComponent on %s"), *GetOwner()->GetName());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TriggerTag))
	{
		ActivatorCount++;

		if (!IsTriggered)
		{
			Trigger(true);
		}		
	}	
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag(TriggerTag))
	{
		ActivatorCount--;

		if (IsTriggered && !ActivatorCount)
		{
			Trigger(false);
		}
	}
}
