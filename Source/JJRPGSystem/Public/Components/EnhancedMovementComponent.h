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

	float DefaultMaxWalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetMaxSprintVelocity", Getter = "GetMaxSprintVelocity"))
	float MaxSprintVelocity = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetCrouchVelocity", Getter = "GetMaxCrouchVelocity"))
	float MaxCrouchVelocity = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedMovement|Speeds",meta= (Setter = "SetCrouchedHalfHeight", Getter = "GetCrouchedHalfHeight"))
	float CrouchedHalfHeight = 44.0f;


public:
	
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
};


