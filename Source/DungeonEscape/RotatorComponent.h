// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API URotatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	float RotateTime = 4.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldPlaySound = false;

	UPROPERTY(EditAnywhere)
	bool ShouldRotateOnBeginPlay = false;

	bool GetShouldRotate();
	void SetShouldRotate(bool NewShouldRotate);

private:
	UPROPERTY(VisibleAnywhere)
	bool ShouldRotate = false;

	FRotator StartRotation;
	FRotator TargetRotation;

	UPROPERTY(VisibleAnywhere)
	bool ReachedTarget = false;
};
