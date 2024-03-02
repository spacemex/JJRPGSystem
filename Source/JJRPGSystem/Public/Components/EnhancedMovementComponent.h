// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent,Blueprintable,BlueprintType))
class JJRPGSYSTEM_API UEnhancedMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnhancedMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;



private:

	float DefaultMaxWalkSpeed = 0.0f; // Saves The MaxWalkSpeed
	bool bCanUnCrouch = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetMaxSprintVelocity", Getter = "GetMaxSprintVelocity"))
	float MaxSprintVelocity = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetCrouchVelocity", Getter = "GetMaxCrouchVelocity"))
	float MaxCrouchVelocity = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetCrouchedHalfHeight", Getter = "GetCrouchedHalfHeight"))
	float CrouchedHalfHeight = 44.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Crouching", meta = (Setter = "SetCanCrouch", Getter = "GetCanCrouch"))
	bool bCanCrouch = true;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Sprinting")
	bool bCanSprint = true;



public:

	// Helpers 
	UFUNCTION(BlueprintPure)
	float GetDefaultWalkSpeed() const
	{
		if (const UWorld* World = GetWorld())
		{
			if (const APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				if (const ACharacter* Player = PlayerController->GetCharacter())
				{
					return Player->GetCharacterMovement()->MaxWalkSpeed;
				}
			}
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void SetDefaultWalkSpeed(float NewMaxWalkSpeed)
	{
		if (NewMaxWalkSpeed < 0)
		{
			return;
		}
		if (const UWorld* World = GetWorld())
		{
			if (const APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				if (const ACharacter* Player = PlayerController->GetCharacter())
				{
					Player->GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
				}
			}
		}
	}

	// Setters
	UFUNCTION(BlueprintCallable)
	void SetMaxSprintVelocity(float NewSprintSpeed){MaxSprintVelocity = NewSprintSpeed;}

	UFUNCTION(BlueprintCallable)
	void SetCrouchVelocity(float NewCrouchSpeed){MaxCrouchVelocity = NewCrouchSpeed;}

	UFUNCTION(BlueprintCallable)
	void SetCrouchedHalfHeight(float NewHalfHeight){CrouchedHalfHeight = NewHalfHeight;}

	UFUNCTION(BlueprintCallable)
	void SetCanCrouch(bool NewCanCrouch){bCanCrouch = NewCanCrouch;}

	//Getters
	UFUNCTION(BlueprintPure)
	float GetMaxSprintVelocity() const {return MaxSprintVelocity;}

	UFUNCTION(BlueprintPure)
	float GetMaxCrouchVelocity() const {return MaxCrouchVelocity;}

	UFUNCTION(BlueprintPure)
	float GetCrouchedHalfHeight() const {return CrouchedHalfHeight;}

	UFUNCTION(BlueprintPure)
	bool GetCanCrouch() const {return bCanCrouch;}
	
	
};


