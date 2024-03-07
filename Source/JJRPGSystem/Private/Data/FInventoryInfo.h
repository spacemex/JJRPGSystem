// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseInventoryItem.h"
#include "FInventoryInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryInfo
{
 GENERATED_BODY()
 
 UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
 TSubclassOf<ABaseInventoryItem> Item;

 UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
 int32 Quantity;
};
