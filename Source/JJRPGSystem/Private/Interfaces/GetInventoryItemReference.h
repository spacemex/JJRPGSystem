// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetInventoryItemReference.generated.h"

class ABaseInventoryItem;
// This class does not need to be modified.
UINTERFACE()
class UGetInventoryItemReference : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JJRPGSYSTEM_API IGetInventoryItemReference
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	ABaseInventoryItem* GetInventoryItemReference();
};
