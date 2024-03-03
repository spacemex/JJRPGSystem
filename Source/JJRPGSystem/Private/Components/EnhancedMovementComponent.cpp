// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EnhancedMovementComponent.h"

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

	UpdateCrouch();
	// ...
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

//void UEnhancedMovementComponent::EndCrouch_Implementation()
//{
	//if (GetIsCrouching())
	//{
		//if (GetCanUnCrouch())
		//{
			//if (const UWorld* World = GetWorld())
			//{
				//if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
				//{
					//SetCanSprint(true);

					//const float InterpSpeed = 0.01;
					//Alpha -= InterpSpeed;
					//Alpha = FMath::Clamp(Alpha - 0.1,0.0f,1.0f);



					//const float NewHeight = FMath::(GetCrouchedHalfHeight(),GetFullHalfHeight(),1 - Alpha);
					//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("New Height: %f"), NewHeight));

					//Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);

					//if (Alpha <= 0.0f)
					//{
						//SetIsCrouching(false);

					//}
				//}
			//}
		//}
	//}

//}

//void UEnhancedMovementComponent::StartCrouch_Implementation()
//{
	//if (GetCanCrouch())
	//{
		//if (GetIsSprinting())
		//{
		//	StopSprint();
		//}
		//if (const UWorld* World = GetWorld())
		//{
			//if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
			//{
				//SetIsCrouching(true);
				//SetCanSprint(false);

				//const float InterpSpeed = 0.01;

				//Alpha += InterpSpeed;

				//Alpha = FMath::Clamp(Alpha,0.0f,1.0f);

				//const float NewHeight = FMath::SmoothStep(GetFullHalfHeight(),GetCrouchedHalfHeight(),Alpha);
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("New Height: %f"), NewHeight));
				
				//Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
			//}
		//}
	//}
//}

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
	if (GetCanSprint())
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
							SetDefaultWalkSpeed(GetMaxSprintVelocity());
							DrainStaminaTimer();
						}
					}
				}	
			}
		}	
	}
	
}

float EaseInExpo(float Alpha) {
    return pow(2, 10 * (Alpha - 1));
}

float EaseOutExpo(float Alpha) {
    return (-pow(2, -10 * Alpha) + 1);
}

void UEnhancedMovementComponent::StartCrouch_Implementation() {
    if (!GetIsCrouching() && GetCanCrouch()) {    
        SetIsCrouching(true);
        SetCanSprint(false);
        Alpha = 0.0f; // Reset Alpha
    }
}

void UEnhancedMovementComponent::EndCrouch_Implementation() {
    if (GetIsCrouching() && GetCanUnCrouch()) {
        SetIsCrouching(false);
        SetCanSprint(true);
        Alpha = -1.0f; // Reset Alpha
    }
}

void UEnhancedMovementComponent::UpdateCrouch() {
    if (const UWorld* World = GetWorld()) {
        Alpha += 0.03f; // Controls the speed of transition, adjust as needed

        if (ACharacter* Player = World->GetFirstPlayerController()->GetCharacter()) {
            if(GetIsCrouching()) {
                float newHeight = FMath::Lerp(GetFullHalfHeight(), GetCrouchedHalfHeight(), EaseInExpo(Alpha));
                Player->GetCapsuleComponent()->SetCapsuleHalfHeight(newHeight);
            } 
            else {
                float newHeight = FMath::Lerp(GetCrouchedHalfHeight(), GetFullHalfHeight(), EaseOutExpo(Alpha));
                Player->GetCapsuleComponent()->SetCapsuleHalfHeight(newHeight);
            }
        }
    }
}