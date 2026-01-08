// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonEscapeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DungeonEscape.h"

#include "CollectableItem.h"
#include "Lock.h"

ADungeonEscapeCharacter::ADungeonEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeonEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeonEscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonEscapeCharacter::LookInput);

		// Interact -- added
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeonEscapeCharacter::Interact);
	
	}
	else
	{
		UE_LOG(LogDungeonEscape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeonEscapeCharacter::Interact()
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractDistance);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	FCollisionShape InteractSphere = FCollisionShape::MakeSphere(InteractSphereRadius);
	// DrawDebugSphere(GetWorld(), End, InteractSphereRadius, 12, FColor::Green, false, 2.0f);

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, InteractSphere);

	if (HasHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor->ActorHasTag("CollectableItem"))
		{
			ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor);
			if (CollectableItem)
			{
				UE_LOG(LogDungeonEscape, Log, TEXT("Collectable Item with name %s"), *CollectableItem->ItemName);
				Inventory.Add(CollectableItem->ItemName);
				HitActor->Destroy();
			}
		}
		else if (HitActor->ActorHasTag("Lock"))
		{
			ALock* LockActor = Cast<ALock>(HitActor);
			if (LockActor)
			{
				// Is the lock empty?
				if (!LockActor->GetIsKeyPlaced())
				{
					UE_LOG(LogDungeonEscape, Log, TEXT("Lock requires key: %s"), *LockActor->KeyItemName);
					
					// Do we have the key? If so, remove it from inventory
					if (Inventory.RemoveSingle(LockActor->KeyItemName))
					{
						// Unlock the lock
						UE_LOG(LogDungeonEscape, Log, TEXT("Unlocking lock with key: %s"), *LockActor->KeyItemName);						
						LockActor->SetIsKeyPlaced(true);
					}
					else
					{
						UE_LOG(LogDungeonEscape, Log, TEXT("You do not have the required key: %s"), *LockActor->KeyItemName);
					}
				}
				else
				{
					// Lock is already unlocked, so we can remove the key and add it back to inventory
					UE_LOG(LogDungeonEscape, Log, TEXT("Removing key from lock: %s"), *LockActor->KeyItemName);
					Inventory.Add(LockActor->KeyItemName);
					LockActor->SetIsKeyPlaced(false);
				}
			}
		}
		else if (HitActor->ActorHasTag("Switch"))
		{
			UE_LOG(LogDungeonEscape, Log, TEXT("'%s' Interacted with Switch '%s'"), *GetNameSafe(this), *GetNameSafe(HitActor));
			if (UTriggerComponent* TriggerComp = HitActor->FindComponentByClass<UTriggerComponent>())
			{
				TriggerComp->Trigger(!TriggerComp->IsTriggered);
			}
			else
			{
				UE_LOG(LogDungeonEscape, Warning, TEXT("Switch '%s' has no Trigger Component"), *GetNameSafe(HitActor));
			}
		}
	}
	else
	{
		UE_LOG(LogDungeonEscape, Log, TEXT("'%s' Interacted with nothing"), *GetNameSafe(this));
	}
}


void ADungeonEscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeonEscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, -LookAxisVector.Y);

}

void ADungeonEscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonEscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeonEscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeonEscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
