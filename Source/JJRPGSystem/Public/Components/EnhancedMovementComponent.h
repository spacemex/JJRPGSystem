// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
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

	FTimerHandle StartSprintHandle;
	FTimerHandle StopSprintHandle;

	float DefaultMaxWalkSpeed = 0.0f; // Saves The MaxWalkSpeed
	
	bool bCanUnCrouch = false;
	bool bIsCrouching = false;
	bool bIsSprinting = false;

	//Movement
	
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

	//Stamina

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Stamina", meta = (Setter = "SetMaxStamina", Getter = "GetMaxStamina"))
	float MaxStamina = 100.0f; // Done

	UPROPERTY(EditDefaultsOnly, Category = "EnahncedMovement|Stamina", meta = (Setter = "SetCurrentStamina", Getter = "GetCurrentStamina"))
	float CurrentStamina = MaxStamina; // Done

	//Timer And Regeneration

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Regeneration",meta = (Setter = "SetCanRegenerateStamina", Getter = "GetCanRegenerateStamina"))
	bool bCanRegenerateStamina = true; // Done

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Rates", meta = (Setter = "SetStaminaDrainRate", Getter = "GetStaminaDrainRate"))
	float StaminaDrainRate = 1.0f; // Done

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Rates", meta = (Setter = "SetStaminaRegenerationRate", Getter = "GetStaminaRegenerationRate"))
	float StaminaRegenerationRate = 1.0f; // Done

	UPROPERTY(EditDefaultsOnly, Category = "EnahncedMovement|Delays", meta = (Setter = "SetStaminaDrainDelay", Getter = "GetStaminaDrainDelay"))
	float StaminaDrainDelay = 1.0f; // Done

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Delays", meta = (Setter = "SetStaminaRegenerationDelay", Getter = "GetStaminaRegenerationDelay"))
	float StaminaRegenerationDelay = 1.0f; // Done

	



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

	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(float NewMaxStamina) {MaxStamina = NewMaxStamina;}

	UFUNCTION(BlueprintCallable)
	void SetCurrentStamina(float NewCurrentStamina) {CurrentStamina = NewCurrentStamina;}

	UFUNCTION(BlueprintCallable)
	void SetCanRegenerateStamina(bool NewCanRegenerateStamina) {bCanRegenerateStamina = NewCanRegenerateStamina;}

	UFUNCTION(BlueprintCallable)
	void SetStaminaDrainRate(float NewStaminaDrainRate) {StaminaDrainRate = NewStaminaDrainRate;}

	UFUNCTION(BlueprintCallable)
	void SetStaminaRegenerationRate(float NewStaminaRegenerationRate) {StaminaRegenerationRate = NewStaminaRegenerationRate;}

	UFUNCTION(BlueprintCallable)
	void SetStaminaDrainDelay(float NewStaminaDrainDelay) {StaminaDrainDelay = NewStaminaDrainDelay;}

	UFUNCTION(BlueprintCallable)
	void SetStaminaRegenerationDelay(float NewStaminaRegenerationDelay) {StaminaRegenerationDelay = NewStaminaRegenerationDelay;}
	


	

	//Getters
	UFUNCTION(BlueprintPure)
	float GetMaxSprintVelocity() const {return MaxSprintVelocity;}

	UFUNCTION(BlueprintPure)
	float GetMaxCrouchVelocity() const {return MaxCrouchVelocity;}

	UFUNCTION(BlueprintPure)
	float GetCrouchedHalfHeight() const {return CrouchedHalfHeight;}

	UFUNCTION(BlueprintPure)
	bool GetCanCrouch() const {return bCanCrouch;}

	UFUNCTION(BlueprintPure)
	bool GetIsCrouching() const {return bIsCrouching;}

	UFUNCTION(BlueprintPure)
	bool GetIsSprinting() const {return bIsSprinting;}

	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const {return MaxStamina;}

	UFUNCTION(BlueprintPure)
	float GetCurrentStamina() const {return CurrentStamina;}

	UFUNCTION(BlueprintPure)
	bool GetCanRegenerateStamina() const {return bCanRegenerateStamina;}

	UFUNCTION(BlueprintPure)
	float GetStaminaDrainRate() const {return StaminaDrainRate;}

	UFUNCTION(BlueprintPure)
	float GetStaminaRegenerationRate() const {return StaminaRegenerationRate;}

	UFUNCTION(BlueprintPure)
	float GetStaminaDrainDelay() const {return StaminaDrainDelay;}

	UFUNCTION(BlueprintPure)
	float GetStaminaRegenerationDelay() const {return StaminaRegenerationDelay;}

	

	//Utility

	UFUNCTION(BlueprintNativeEvent)
	void StartSprint();

	UFUNCTION(BlueprintNativeEvent)
	void StopSprint();

	UFUNCTION(BlueprintNativeEvent)
	void StartCrouch();

	UFUNCTION(BlueprintNativeEvent)
	void EndCrouch();

	UFUNCTION()
	void StartStaminaDrain();

	UFUNCTION()
	void StartStaminaRegeneration();

	
	
	
};


