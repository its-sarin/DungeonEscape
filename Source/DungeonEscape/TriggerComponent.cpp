// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ActorsToTrigger.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorsToTrigger is not set in TriggerComponent on %s"), *GetOwner()->GetName());
	}
	else
	{		
		// Iterate through ActorsToTrigger and populate MoversToTrigger, FallersToTrigger, RotatorsToTrigger
		for (AActor* Actor : ActorsToTrigger)
		{
			UMover* FoundMover = Actor->FindComponentByClass<UMover>();
			if (FoundMover)
			{
				MoversToTrigger.Add(FoundMover);
			}
			UFaller* FoundFaller = Actor->FindComponentByClass<UFaller>();
			if (FoundFaller)
			{
				FallersToTrigger.Add(FoundFaller);
			}
			URotatorComponent* FoundRotator = Actor->FindComponentByClass<URotatorComponent>();
			if (FoundRotator)
			{
				RotatorsToTrigger.Add(FoundRotator);
			}
			ULightComponent* FoundLight = Actor->FindComponentByClass<ULightComponent>();
			if (FoundLight)
			{
				LightsToToggle.Add(FoundLight);
			}
		}

		if (MoversToTrigger.Num() == 0 && FallersToTrigger.Num() == 0 && RotatorsToTrigger.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Mover, Faller, or RotatorComponent on any ActorToTrigger in TriggerComponent on %s"), *GetOwner()->GetName());
		}



		/*Mover = ActorToTrigger->FindComponentByClass<UMover>();
		Faller = ActorToTrigger->FindComponentByClass<UFaller>();
		RotatorComp = ActorToTrigger->FindComponentByClass<URotatorComponent>();*/
		
		/*if (!Mover && !Faller)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Mover or Faller on ActorToTrigger in TriggerComponent on %s"), *GetOwner()->GetName());
		}*/
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

	if (MoversToTrigger.Num() > 0)
	{
		for (UMover* Mover : MoversToTrigger)
		{
			if (Mover)
			{
				Mover->SetShouldMove(IsTriggered);
			}
		}
	}

	if (FallersToTrigger.Num() > 0)
	{
		for (UFaller* Faller : FallersToTrigger)
		{
			if (Faller)
			{
				Faller->SetShouldFall(IsTriggered);
			}
		}
	}

	if (RotatorsToTrigger.Num() > 0)
	{
		for (URotatorComponent* RotatorComp : RotatorsToTrigger)
		{
			if (RotatorComp)
			{
				RotatorComp->SetShouldRotate(IsTriggered);
			}
		}
	}

	if (LightsToToggle.Num() > 0 && IsTriggered)
	{
		for (ULightComponent* LightComp : LightsToToggle)
		{
			if (LightComp)
			{
				LightComp->SetVisibility(!LightComp->IsVisible());
			}
		}
	}

	/*if (Mover)
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
	}*/
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
