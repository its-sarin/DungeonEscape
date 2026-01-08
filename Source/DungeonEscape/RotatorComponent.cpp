// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatorComponent.h"

// Sets default values for this component's properties
URotatorComponent::URotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StartRotation = GetOwner()->GetActorRotation();
	if (ShouldRotateOnBeginPlay) SetShouldRotate(true);
}


// Called every frame
void URotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ShouldRotate) return;

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	ReachedTarget = CurrentRotation.Equals(TargetRotation);

	if (!ReachedTarget)
	{
		float Speed = RotationOffset.GetManhattanDistance(StartRotation) / RotateTime;
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
		GetOwner()->SetActorRotation(NewRotation);
	}
	else
	{
		SetShouldRotate(false);
	}
}

bool URotatorComponent::GetShouldRotate()
{
	return ShouldRotate;
}

void URotatorComponent::SetShouldRotate(bool NewShouldRotate)
{
	ShouldRotate = NewShouldRotate;

	if (ShouldRotate)
	{
		TargetRotation = StartRotation + RotationOffset;
	}
}

