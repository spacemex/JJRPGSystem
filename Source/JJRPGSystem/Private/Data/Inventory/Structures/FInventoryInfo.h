// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Objects/BaseInventoryItem.h"
#include "FInventoryInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct FInventoryInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UBaseInventoryItem> Item;

	UPROPERTY(EditDefaultsOnly, Category = "Quantity")
	int32 Quantity;
	
};
