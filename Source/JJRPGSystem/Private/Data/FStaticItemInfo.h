// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BasePickupItem.h"
#include "FStaticItemInfo.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FStaticItemInfo : public FTableRowBase
{
 GENERATED_BODY()

 UPROPERTY(EditDefaultsOnly, Category = "Required")
 TSubclassOf<ABasePickupItem> PickupRef;

 UPROPERTY(EditDefaultsOnly, Category = "Weight")
 float ItemWeight = 0.0f;


};
