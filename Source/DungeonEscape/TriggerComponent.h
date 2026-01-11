// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Mover.h"
#include "Faller.h"
#include "RotatorComponent.h"
#include "Components/LightComponent.h"


#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEONESCAPE_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UPROPERTY(EditAnywhere)
	//AActor* ActorToTrigger;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToTrigger;

	UPROPERTY(VisibleAnywhere)
	TArray<UMover*> MoversToTrigger;
	UPROPERTY(VisibleAnywhere)
	TArray<UFaller*> FallersToTrigger;
	UPROPERTY(VisibleAnywhere)
	TArray<URotatorComponent*> RotatorsToTrigger;
	UPROPERTY(VisibleAnywhere)
	TArray<ULightComponent*> LightsToToggle;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate")
	bool IsPressurePlate = false;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate", meta = (EditCondition = "IsPressurePlate", EditConditionHides))
	bool StayTriggeredWhenActorRemoved = false;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate")
	FName TriggerTag;

	UPROPERTY(VisibleAnywhere)
	bool IsTriggered = false;

	void Trigger(bool NewTriggerValue);

	/** called when something enters the sphere component */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	int32 ActivatorCount = 0;
};
