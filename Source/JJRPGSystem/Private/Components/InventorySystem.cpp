// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventorySystem.h"

#include "Actors/BasePickupItem.h"
#include "Data/FStaticItemInfo.h"


// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventorySystem::UpdateWeight(FName RowName)
{
	float LocalWeightCalc = 0.0f;

	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	for (int32 i = 0; i < Contents.Num(); i++)
	{
		UClass* ItemSubClass = Contents[i].Item;
		if (ItemSubClass != nullptr && ItemSubClass->IsChildOf<ABaseInventoryItem>())
		{
			ABaseInventoryItem* SpawnedItem = GetWorld()->SpawnActor<ABaseInventoryItem>(ItemSubClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            
			if (SpawnedItem)
			{
				ABaseInventoryItem* ItemRef = IGetInventoryItemReference::Execute_GetInventoryItemReference(SpawnedItem);
				if (ItemRef)
				{
					FStaticItemInfo* ItemInfo = ItemsTable->FindRow<FStaticItemInfo>(ItemRef->RowName, TEXT("UpdateWeight"));
					if (ItemInfo)
					{
						LocalWeightCalc += (ItemInfo->ItemWeight * Contents[i].Quantity);
					}
				}
				SpawnedItem->Destroy();
			}
		}
	}
	CurrentWeight = LocalWeightCalc;
}

bool UInventorySystem::AddItem_Implementation(ABasePickupItem* ItemPickup,FName RowName)
{
	bool OpSuccess = false; // Changed To True If Item Successfully Added

	// Add Item Functionality
	const IGetPickupItemReference* PickupInterface = Cast<IGetPickupItemReference>(ItemPickup); // Get ABaseItemPickup Instance Via Interface
	if (const ABasePickupItem* PickupItem = PickupInterface->Execute_GetPickupItemReference(ItemPickup); ItemPickup && PickupInterface && PickupItem) // Sanity Checks
	{
		bool ItemFound = false; // Only Becomes True If Item is In Inventory
		for (int32 i = 0; i < Contents.Num(); i++)
		{
			if (Contents[i].Item == PickupItem->InventoryItem)
			{
				Contents[i].Quantity += PickupItem->Quantity;
				ItemFound = true; // Found Item
				OpSuccess = true; // It Was A Success Adding The Item Or Updating A Existing one

				break;
			}
		}
		if (!ItemFound) // If No Item Was Found Earlier It Adds A New Item 
		{
			FInventoryInfo NewItemInfo;

			NewItemInfo.Item = PickupItem->InventoryItem;
			NewItemInfo.Quantity = PickupItem->Quantity;
			
			Contents.Add(NewItemInfo); // Adds New Item.
			OpSuccess = true; // It Was A Success Adding The Item Or Updating A Existing one

		}
	}
	if (true) // If The Item Was Added Successfully We Update Our Weight 
	{
		
		UpdateWeight(RowName);
	}
	return OpSuccess; 
}


