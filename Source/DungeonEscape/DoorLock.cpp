// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorLock.h"

// Sets default values
ADoorLock::ADoorLock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Component"));
	TriggerComp->SetupAttachment(RootComp);

	LockClosedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock Closed Mesh"));
	LockClosedMesh->SetupAttachment(RootComp);

	LockOpenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock Open Mesh"));
	LockOpenMesh->SetupAttachment(RootComp);

	Tags.Add(FName("DoorLock"));
}

// Called when the game starts or when spawned
void ADoorLock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADoorLock::GetIsKeyPlaced()
{
	return IsKeyPlaced;
}

void ADoorLock::SetIsKeyPlaced(bool NewIsKeyPlaced)
{
	IsKeyPlaced = NewIsKeyPlaced;
	TriggerComp->Trigger(NewIsKeyPlaced);
	LockClosedMesh->SetVisibility(!NewIsKeyPlaced);
	LockOpenMesh->SetVisibility(NewIsKeyPlaced);

	if (NewIsKeyPlaced)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoorLock: Key placed, lock opened."));

		// Enable Simulate Physics on the LockOpenMesh to allow it to fall
		LockOpenMesh->SetSimulatePhysics(true);
	}

}