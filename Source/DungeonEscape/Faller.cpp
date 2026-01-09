// Fill out your copyright notice in the Description page of Project Settings.


#include "Faller.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UFaller::UFaller()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFaller::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFaller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UFaller::GetShouldFall()
{
	return ShouldFall;
}

void UFaller::SetShouldFall(bool NewShouldFall)
{
	ShouldFall = NewShouldFall;

	if (ShouldFall)
	{
		// Enable simulate physics on root component
		if (AActor* Owner = GetOwner())
		{
			if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
			{
				if (ShouldPlaySound)
				{
					UAudioComponent* AudioComp = Owner->FindComponentByClass<UAudioComponent>();
					if (AudioComp)
					{
						AudioComp->Play();
					}
				}

				RootComp->SetSimulatePhysics(true);
			}
		}
	}
}

