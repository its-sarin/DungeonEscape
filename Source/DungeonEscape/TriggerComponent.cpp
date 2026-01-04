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
		if (!Mover)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover component not found on ActorToTrigger in TriggerComponent on %s"), *GetOwner()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Mover component found on ActorToTrigger in TriggerComponent on %s"), *GetOwner()->GetName());
			Mover->ShouldMove = true;
		}
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
