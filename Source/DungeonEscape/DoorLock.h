// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "DoorLock.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADoorLock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorLock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LockClosedMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LockOpenMesh;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere)
	FString KeyItemName;

	bool GetIsKeyPlaced();
	void SetIsKeyPlaced(bool NewIsKeyPlaced);

private:
	UPROPERTY(VisibleAnywhere)
	bool IsKeyPlaced = false;

};
