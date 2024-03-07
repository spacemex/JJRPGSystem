// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/FInventoryInfo.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.generated.h"


class ABasePickupItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JJRPGSYSTEM_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventorySystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemsTable = nullptr;

	//The Array That Holds The Contents Of Our Inventory.
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryInfo> Contents;

	UPROPERTY(EditDefaultsOnly)
	float CurrentWeight = 0.0f;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool AddItem(ABasePickupItem* ItemPickup, FName RowName);
	
	UFUNCTION(BlueprintPure)
	TArray<FInventoryInfo> GetInventoryContents()
	{
		return Contents;
	}

	UFUNCTION(BlueprintCallable)
	void UpdateWeight(FName RowName);

	UFUNCTION(BlueprintPure)
	float GetCurrentWeight() const {return CurrentWeight;}
};
