// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cfloat>
#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable,BlueprintType)
class JJRPGSYSTEM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
private:

	FTimerHandle HealthRegenerationHandle;
	FTimerHandle ArmorRegenerationHandle;
	
	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Health",meta = (Setter = "SetMaxHealth",Getter = "GetMaxHealth"))
	float MaxHealth = 100;
	
	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Health",meta = (Setter = "SetCurrentHealth",Getter = "GetCurrentHealth"))
	float CurrentHealth = 100;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Health|Regeneration",meta = (Setter = "SetRegenerateHealth",Getter = "GetRegenerateHealth"))
	bool RegenerateHealth = true;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Health|Regeneration",meta = (Setter = "SetHealthRegenerationSpeed",Getter = "GetHealthRegenerationSpeed"))
	float HealthRegenerationSpeed = 2.5;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Armor",meta = (Setter = "SetMaxArmor",Getter = "GetMaxArmor"))
	float MaxArmor = 100;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Armor",meta = (Setter = "SetCurrentArmor",Getter = "GetCurrentArmor"))
	float CurrentArmor = 0;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Armor|Regeneration",meta = (Setter = "SetRegenerateArmor",Getter = "GetRegenerateArmor"))
	bool RegenerateArmor = false;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Armor|Regeneration",meta = (Setter = "SetArmorRegenerationSpeed",Getter = "GetArmorRegenerationSpeed"))
	float ArmorRegenerationSpeed = 5.5;

	UPROPERTY(EditDefaultsOnly,Category="HealthSystem|Damage",meta = (Setter = "GetRegenerateAfterDamageDelay",Getter = "SetRegenerateAfterDamageDelay"))
	float RegenerateAfterDamageDelay = 10.0;

public:
	//Functions
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth) {MaxHealth = FMath::Clamp(NewMaxHealth,0.0f, FLT_MAX);}

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const {return CurrentHealth;}

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewCurrentHealth){CurrentHealth = FMath::Clamp(NewCurrentHealth,0.0f,FLT_MAX);} 

	UFUNCTION(BlueprintPure)
	bool GetRegenerateHealth() const {return RegenerateHealth;}

	/**
	 * Sets The Ability To Regen Health
	 * And Start The Timer For Regeneration 
	 * @param NewRegenerateHealth 
	 */
	UFUNCTION(BlueprintCallable)
	void SetRegenerateHealth(bool NewRegenerateHealth)
	{
		{
			RegenerateHealth = NewRegenerateHealth;
			if (GetRegenerateHealth())
			{
				if (const UWorld* World = GetWorld())
				{
					if (World->GetTimerManager().IsTimerActive(HealthRegenerationHandle))
					{
						return;
					}
					World->GetTimerManager().SetTimer(HealthRegenerationHandle,this,
						&UHealthComponent::RegenerateHealthTimer,RegenerateAfterDamageDelay,false);
				}
			}
		}
	}
	
	UFUNCTION(BlueprintPure)
	float GetHealthRegenerationSpeed() const {return HealthRegenerationSpeed;}

	UFUNCTION(BlueprintPure)
	float GetMaxArmor() const {return MaxArmor;}

	UFUNCTION(BlueprintCallable)
	void SetMaxArmor(float NewMaxArmor) {MaxArmor = FMath::Clamp(NewMaxArmor,0.0f,FLT_MAX);}

	UFUNCTION(BlueprintPure)
	float GetCurrentArmor() const {return CurrentArmor;}

	UFUNCTION(BlueprintCallable)
	void SetCurrentArmor(float NewCurrentArmor) {CurrentArmor = FMath::Clamp(NewCurrentArmor,0.0f,FLT_MAX);}

	UFUNCTION(BlueprintPure)
	bool GetRegenerateArmor() const {return RegenerateArmor;}

	/**
	 * Sets The Ability To Regen Armor
	 * And Start The Timer For Regeneration 
	 * @param NewRegenerateArmor Default = false
	 */
	UFUNCTION(BlueprintCallable)
	void SetRegenerateArmor(bool NewRegenerateArmor)
	{
		RegenerateArmor = NewRegenerateArmor;
		if (GetRegenerateArmor())
		{
			if (const UWorld* World = GetWorld())
			{
				if (World->GetTimerManager().IsTimerActive(ArmorRegenerationHandle))
				{
					return;
				}
				World->GetTimerManager().SetTimer(ArmorRegenerationHandle,this,
					&UHealthComponent::RegenerateArmorTimer,RegenerateAfterDamageDelay,false);
			}
		}
	}

	UFUNCTION(BlueprintPure)
	float GetArmorRegenerationSpeed() const {return ArmorRegenerationSpeed;}

	UFUNCTION(BlueprintCallable)
	void SetArmorRegenerationSpeed(float NewArmorRegenerationSpeed) {ArmorRegenerationSpeed = FMath::Clamp(NewArmorRegenerationSpeed,0.0f,FLT_MAX);}

	UFUNCTION(BlueprintPure)
	float GetRegenerateAfterDamageDelay() const {return RegenerateAfterDamageDelay;}

	UFUNCTION(BlueprintCallable)
	void SetRegenerateAfterDamageDelay(float NewRegenerateAfterDamageDelay) {RegenerateAfterDamageDelay = FMath::Clamp(NewRegenerateAfterDamageDelay,0.0f,FLT_MAX);}


	/**
	 * Damages The Armor Before Applying LeftOver Damage To The Health.
	 * @param AfflictedDamage Amount Of Damage To Deal To The Target.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AfflictDamage(float AfflictedDamage);

	/**
	 * Adds Health And Or Armor To The Player.
	 * (Negatives Do Not Work 0 Do Do Nothing).
	 * @param HealthToAdd Amount Of Health To Add To The Player.
	 * @param ArmorToAdd  Amount Of Armor To Add To The Player.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddHealthAndArmor(float HealthToAdd, float ArmorToAdd);

	/**
	 * Damages The Players Armor If Any Exists.
	 * @param Damage  Amount Of Damage To Inflect.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DamageArmor( float Damage);

	/**
	 * Damages The Players Health And Ignores Armor.
	 * @param Damage Amount Of Damage To Inflect.
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DamageHealth(float Damage);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void RegenerateArmorEvent();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void RegenerateHealthEvent();

	UFUNCTION(BlueprintNativeEvent)
	void RegenerateArmorTimer();

	UFUNCTION(BlueprintNativeEvent)
	void RegenerateHealthTimer();
	
};
