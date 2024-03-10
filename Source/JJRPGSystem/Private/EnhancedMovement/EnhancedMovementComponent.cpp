// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedMovement/EnhancedMovementComponent.h"

#include "Components/CapsuleComponent.h"
// Sets default values for this component's properties
UEnhancedMovementComponent::UEnhancedMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}

// Called when the game starts
void UEnhancedMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetDefaultWalkSpeed(GetMaxWalkVelocity());
	GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCapsuleComponent()->SetCapsuleHalfHeight(GetFullHalfHeight());
	// ...



	
}



// Called every frame
void UEnhancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UpdateCrouch();
	// ...
}

void UEnhancedMovementComponent::HandleProgress()
{
	ElapsedTime = FMath::Min(TimelineLengthInSeconds, ElapsedTime + 0.01f);
	const float LerpValue = FMath::Lerp(OriginalCapsuleHalfHeight, TargetCapsuleHalfHeight, ElapsedTime/TimelineLengthInSeconds);
	AdjustCapsuleHalfHeight(LerpValue);
	
	if (ElapsedTime >= TimelineLengthInSeconds)
	{
		GetWorld()->GetTimerManager().ClearTimer(CrouchTimerHandle);
	}
}

void UEnhancedMovementComponent::AdjustCapsuleHalfHeight(float NewHalfHeight)
{
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner())) 
	{
		Character->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
	}
}

void UEnhancedMovementComponent::DrainStaminaTimer_Implementation()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(StartSprintHandle,this,
			&UEnhancedMovementComponent::StaminaDrain,StaminaDrainDelay,true);
	}
}

void UEnhancedMovementComponent::RegenerateStaminaTimer_Implementation()
{
	if (GetCanRegenerateStamina())
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(StopSprintHandle,this,
				&UEnhancedMovementComponent::StaminaRegeneration,StaminaRegenerationDelay,true);
		}
	}
}


void UEnhancedMovementComponent::StaminaRegeneration_Implementation()
{
	if (!GetIsSprinting() && GetCurrentStamina() < GetMaxStamina() && GetCanRegenerateStamina())
	{
		SetIsRegeneratingStamina(true);
		if (GetIsRegeneratingStamina())
		{
			SetCurrentStamina(FMath::Clamp(GetCurrentStamina() + StaminaRegenerationRate,0.0f, MaxStamina));
			if (GetIsDrainingStamina())
			{
				SetIsDrainingStamina(false);
			}
			if (GetCurrentStamina() == GetMaxStamina())
			{
				SetIsRegeneratingStamina(false);
			}
		}

	}
}

void UEnhancedMovementComponent::StaminaDrain_Implementation()
{
	if (GetIsSprinting() && GetVelocity() > 10 && GetCurrentStamina() > 0.0f)
	{
		SetIsDrainingStamina(true);
		if (GetIsDrainingStamina())
		{
			SetCurrentStamina(FMath::Clamp(GetCurrentStamina() - GetStaminaDrainRate(),0.0f, GetMaxStamina()));
			if (GetIsRegeneratingStamina())
			{
				SetIsRegeneratingStamina(false);
			}
		}
		if (GetCurrentStamina() == 0.0f)
		{
			StopSprint();
			SetIsDrainingStamina(false);
		}
	}
}

void UEnhancedMovementComponent::EndCrouch_Implementation()
{
	if (GetIsCrouching())
	{
		if (GetCanUnCrouch())
		{
			if (GetIsSprinting())
			{
				SetDefaultWalkSpeed(MaxSprintVelocity);
				SetCanSprint(true);
			}
			const UWorld* World = GetWorld();
			const ACharacter* Character = World->GetFirstPlayerController()->GetCharacter();

			OriginalCapsuleHalfHeight = Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			TargetCapsuleHalfHeight = GetFullHalfHeight();
			ElapsedTime = 0.0f;

			World->GetTimerManager().SetTimer(CrouchTimerHandle, this, &UEnhancedMovementComponent::HandleProgress, 0.01f, true);

			SetDefaultWalkSpeed(MaxWalkVelocity);
			SetIsCrouching(false);
			SetCanSprint(true);
		}
	}
}

void UEnhancedMovementComponent::StartCrouch_Implementation()
{
	if (GetCanCrouch())
	{
		if (GetIsSprinting())
		{
			StopSprint();
		}
		const UWorld* World = GetWorld();
		const ACharacter* Character = World->GetFirstPlayerController()->GetCharacter();

		OriginalCapsuleHalfHeight= Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		TargetCapsuleHalfHeight = GetCrouchedHalfHeight();
		ElapsedTime = 0.0f;

		World->GetTimerManager().SetTimer(CrouchTimerHandle, this, &UEnhancedMovementComponent::HandleProgress, 0.01f, true);
		
		SetDefaultWalkSpeed(GetMaxCrouchVelocity());
		SetIsCrouching(true);
		SetCanSprint(false);
	}
}

void UEnhancedMovementComponent::StopSprint_Implementation()
{
	if (GetIsSprinting())
	{
		if (const UWorld* World = GetWorld())
		{
			if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
			{
				SetCanCrouch(true);
				SetIsSprinting(false);
				SetIsDrainingStamina(true);
				SetDefaultWalkSpeed(GetMaxWalkVelocity());
				RegenerateStaminaTimer();
				SetCanSprint(true);
			}
		}
	}
}

void UEnhancedMovementComponent::StartSprint_Implementation()
{
	if (GetIsCrouching())
	{
		if (GetCanUnCrouch())
		{
			EndCrouch();
		}
	}
	if (GetCanSprint() && !GetIsCrouching())
	{
		if (GetVelocity() > 10.0f)
		{
			if (const float MovementAngle = GetMovementAngle(); 0.0f <= MovementAngle && MovementAngle <= 90.0f)
			{
				if (GetCurrentStamina() > 0.0f)
				{
					if (const UWorld* World = GetWorld())
					{
						if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
						{
							SetIsSprinting(true);
							SetCanCrouch(false);
							SetCanSprint(false);
							SetDefaultWalkSpeed(GetMaxSprintVelocity());
							DrainStaminaTimer();
						}
					}
				}	
			}
		}	
	}
}