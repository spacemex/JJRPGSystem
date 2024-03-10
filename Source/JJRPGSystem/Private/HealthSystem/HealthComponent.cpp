// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem/HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	RegenerateArmorTimer_Implementation();
	RegenerateHealthTimer_Implementation();
	
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::RegenerateHealthTimer_Implementation()
{
	if (GetRegenerateHealth())
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(HealthRegenerationHandle,this,
				&UHealthComponent::RegenerateHealthEvent,HealthRegenerationSpeed,true);
		}
	}
}

void UHealthComponent::RegenerateArmorTimer_Implementation()
{
	if (GetRegenerateArmor())
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ArmorRegenerationHandle,this,
				&UHealthComponent::RegenerateArmorEvent,ArmorRegenerationSpeed,true);
		}
	}
}

void UHealthComponent::RegenerateHealthEvent_Implementation()
{
	if (GetRegenerateHealth() && GetCurrentHealth() < GetMaxHealth())
	{
		AddHealthAndArmor(1.0f,0.0f);
	}
}

void UHealthComponent::RegenerateArmorEvent_Implementation()
{
	if (GetRegenerateArmor() && GetCurrentArmor() < GetMaxArmor())
	{
		AddHealthAndArmor(0.0f,1.0f);
	}
}


void UHealthComponent::DamageHealth_Implementation(float Damage)
{
	if (Damage < 0){return;}
	const UWorld* World = GetWorld();
	if (!World){return;}
	if (GetCurrentHealth() >= Damage)
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - Damage,0.0f,GetMaxHealth()));
		if (World->GetTimerManager().IsTimerActive(HealthRegenerationHandle))
		{
			World->GetTimerManager().ClearTimer(HealthRegenerationHandle);
		}
		if (GetRegenerateHealth())
		{
			World->GetTimerManager().SetTimer(HealthRegenerationHandle,this,
				&UHealthComponent::RegenerateHealthTimer,RegenerateAfterDamageDelay,false);
		}
	}
}

void UHealthComponent::DamageArmor_Implementation(float Damage)
{
	if (Damage < 0){return;}
	const UWorld* World = GetWorld();
	if (!World){return;}
	if (GetCurrentArmor() >= Damage)
	{
		SetCurrentArmor(FMath::Clamp(GetCurrentArmor() - Damage,0.0f,GetMaxArmor()));
		if (World->GetTimerManager().IsTimerActive(ArmorRegenerationHandle))
		{
			World->GetTimerManager().ClearTimer(ArmorRegenerationHandle);
		}
		if (GetRegenerateArmor())
		{
			World->GetTimerManager().SetTimer(ArmorRegenerationHandle,this,
				&UHealthComponent::RegenerateArmorTimer,RegenerateAfterDamageDelay,false);
		}
	}
}

void UHealthComponent::AddHealthAndArmor_Implementation(float HealthToAdd, float ArmorToAdd)
{
	SetCurrentHealth(FMath::Clamp(GetCurrentHealth() + HealthToAdd,0.0f,GetMaxHealth()));
	SetCurrentArmor(FMath::Clamp(GetCurrentArmor() + ArmorToAdd,0.0f,GetMaxArmor()));
}

void UHealthComponent::AfflictDamage_Implementation(const float AfflictedDamage)
{
	if (AfflictedDamage < 0){return;}
	const UWorld* World = GetWorld();
	if (!World){return;}
	if (CurrentArmor > AfflictedDamage)
	{
		SetCurrentArmor(GetCurrentArmor() - AfflictedDamage);

		if (World->GetTimerManager().IsTimerActive(ArmorRegenerationHandle))
		{
			World->GetTimerManager().ClearTimer(ArmorRegenerationHandle);

		}
		if (GetRegenerateArmor())
		{
			World->GetTimerManager().SetTimer(ArmorRegenerationHandle,this,
				&UHealthComponent::RegenerateArmorTimer_Implementation,RegenerateAfterDamageDelay,false);
		}
	}
	else
	{
		const float DamageAfterArmor = AfflictedDamage - GetCurrentArmor();
		SetCurrentArmor(0.0f);

		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - DamageAfterArmor,0.0f,GetMaxHealth()));

		if (World->GetTimerManager().IsTimerActive(ArmorRegenerationHandle))
		{
			World->GetTimerManager().ClearTimer(ArmorRegenerationHandle);
		}
		if (World->GetTimerManager().IsTimerActive(HealthRegenerationHandle))
		{
			World->GetTimerManager().ClearTimer(HealthRegenerationHandle);
		}
		if (GetRegenerateArmor())
		{
			World->GetTimerManager().SetTimer(ArmorRegenerationHandle,this,
				&UHealthComponent::RegenerateArmorTimer_Implementation,RegenerateAfterDamageDelay,false);
		}
		if (GetRegenerateHealth())
		{
			World->GetTimerManager().SetTimer(HealthRegenerationHandle,this,
				&UHealthComponent::RegenerateHealthTimer_Implementation,RegenerateAfterDamageDelay,false);
		}
	}
}


