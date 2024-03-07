﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetPickupItemReference.generated.h"


class ABasePickupItem;
// This class does not need to be modified.
UINTERFACE()
class UGetPickupItemReference : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JJRPGSYSTEM_API IGetPickupItemReference
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	ABasePickupItem* GetPickupItemReference();
};
