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
	}

	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator") && Mover)
	{
		Mover->ShouldMove = true;
	}	
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator") && Mover)
	{
		Mover->ShouldMove = false;
	}
}
