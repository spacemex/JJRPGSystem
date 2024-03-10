// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FStaticItemInfo.generated.h"

class ABasePickupItem;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStaticItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Required")
	TSubclassOf<ABasePickupItem> PickupRef;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Display")
	FName ItemName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Display")
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Display")
	UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Display|WIP")
	UStaticMesh* ItemInspectionMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Encumberance")
	int32 ItemWeight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shop")
	float ItemSellValue;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shop")
	float ItemBuyValue;
};
