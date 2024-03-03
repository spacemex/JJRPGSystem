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
	void UpdateCrouch();

private:

	FTimerHandle StartSprintHandle;
	FTimerHandle StopSprintHandle;

	float Alpha = 0.0f;
	bool bCanUnCrouch = true;
	bool bIsCrouching = false;
	bool bIsSprinting = false;
	bool bIsRegeneratingStamina = false;
	bool bIsDrainingStamina = false;

	//Movement
	
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds")
	float MaxWalkVelocity = 500.0f;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetMaxSprintVelocity", Getter = "GetMaxSprintVelocity"))
	float MaxSprintVelocity = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetCrouchVelocity", Getter = "GetMaxCrouchVelocity"))
	float MaxCrouchVelocity = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Crouching",meta= (Setter = "SetCrouchedHalfHeight", Getter = "GetCrouchedHalfHeight"))
	float CrouchedHalfHeight = 44.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Crouching", meta = (Setter = "SetCanCrouch", Getter = "GetCanCrouch"))
	bool bCanCrouch = true;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Sprinting")
	bool bCanSprint = true;

	//Stamina

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Stamina", meta = (Setter = "SetMaxStamina", Getter = "GetMaxStamina"))
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Stamina", meta = (Setter = "SetCurrentStamina", Getter = "GetCurrentStamina"))
	float CurrentStamina = MaxStamina;

	//Timer And Regeneration

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Regeneration",meta = (Setter = "SetCanRegenerateStamina", Getter = "GetCanRegenerateStamina"))
	bool bCanRegenerateStamina = true;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Rates", meta = (Setter = "SetStaminaDrainRate", Getter = "GetStaminaDrainRate"))
	float StaminaDrainRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Rates", meta = (Setter = "SetStaminaRegenerationRate", Getter = "GetStaminaRegenerationRate"))
	float StaminaRegenerationRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Delays", meta = (Setter = "SetStaminaDrainDelay", Getter = "GetStaminaDrainDelay"))
	float StaminaDrainDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Delays", meta = (Setter = "SetStaminaRegenerationDelay", Getter = "GetStaminaRegenerationDelay"))
	float StaminaRegenerationDelay = 1.0f;

	//Misc

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Crouching")
	float FullHalfHeight = 88.0f;



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
		return 0.0f;
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

	UFUNCTION(BlueprintPure)
	float GetVelocity() const
	{
		if (const UWorld* World = GetWorld())
		{
			if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
			{
				return Player->GetVelocity().Length();
			}
		}
		UE_LOG(LogTemp,Error,TEXT("Failed To Get Velocity"))
		return 0.0f;
	}

	/**
	 * @brief Get The Players Facing Angle.
	 * So, the resulting angle is the angle between the direction of the player's movement and the direction the player is facing,
	 * in degrees. It will be 0 if the player is moving directly forward,
	 * greater than 0 and less than 90 if the player is moving forward but at an angle,
	 * 90 if the player is moving directly sideways, greater than 90 and less than 180 if the player is moving backwards at an angle,
	 * and 180 if the player is moving directly backwards.
	 */
	UFUNCTION(BlueprintPure)
	float GetMovementAngle() const
	{
		if (const UWorld* World = GetWorld())
		{
			if (const ACharacter* Player = World->GetFirstPlayerController()->GetCharacter())
			{
				const FVector VelocityDirection = Player->GetVelocity().GetSafeNormal();
				const FVector FacingDirection = Player->GetActorForwardVector();
				const float DotProduct = FVector::DotProduct(VelocityDirection,FacingDirection);
				const float MovementAngle = FMath::Acos(DotProduct);
				return FMath::RadiansToDegrees(MovementAngle);
			}
		}
		UE_LOG(LogTemp,Error,TEXT("Failed To get Movement Angle"));
		return 0.0f;
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

	UFUNCTION(BlueprintCallable)
	void SetIsRegeneratingStamina(bool NewRegeneratingStamina) {bIsRegeneratingStamina = NewRegeneratingStamina;}

	UFUNCTION(BlueprintCallable)
	void SetIsDrainingStamina(bool NewDrainingStamina) {bIsDrainingStamina = NewDrainingStamina;}

	UFUNCTION(BlueprintCallable)
	void SetCanUnCrouch(bool NewCanUnCrouch) {bCanUnCrouch = NewCanUnCrouch;}

	UFUNCTION(BlueprintCallable)
	void SetCanSprint(bool NewCanSprint) {bCanSprint = NewCanSprint;}

	UFUNCTION(BlueprintCallable)
	void SetIsSprinting(bool NewIsSprinting) {bIsSprinting = NewIsSprinting;}

	UFUNCTION(BlueprintCallable)
	void SetMaxWalkVelocity(float NewMaxWalkVelocity) {MaxWalkVelocity = NewMaxWalkVelocity;}

	UFUNCTION(BlueprintCallable)
	void SetIsCrouching(bool NewIsCrouching) {bIsCrouching = NewIsCrouching;}

	UFUNCTION(BlueprintCallable)
	void SetFullHalfHeight(float NewHalfHeight) {FullHalfHeight = NewHalfHeight;}
	


	

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

	UFUNCTION(BlueprintPure)
	bool GetIsRegeneratingStamina() const {return bIsRegeneratingStamina;}

	UFUNCTION(BlueprintPure)
	bool GetIsDrainingStamina() const {return bIsDrainingStamina;}

	UFUNCTION(BlueprintPure)
	bool GetCanUnCrouch() const {return bCanUnCrouch;}

	UFUNCTION(BlueprintPure)
	bool GetCanSprint() const {return bCanSprint;}

	UFUNCTION(BlueprintPure)
	float GetMaxWalkVelocity() const {return MaxWalkVelocity;}

	UFUNCTION(BlueprintPure)
	float GetFullHalfHeight() const {return FullHalfHeight;}
	


	

	//Utility

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void StartSprint();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void StopSprint();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void StartCrouch();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void EndCrouch();

	UFUNCTION(BlueprintNativeEvent)
	void StaminaDrain(); // Event

	UFUNCTION(BlueprintNativeEvent)
	void StaminaRegeneration(); // Event

	UFUNCTION(BlueprintNativeEvent)
	void DrainStaminaTimer(); // Timer

	UFUNCTION(BlueprintNativeEvent)
	void RegenerateStaminaTimer(); // Timer

	
	
	
};


