// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseInventoryItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class JJRPGSYSTEM_API UBaseInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category = "Inventory")
	FDataTableRowHandle ItemInfo;
	
};
