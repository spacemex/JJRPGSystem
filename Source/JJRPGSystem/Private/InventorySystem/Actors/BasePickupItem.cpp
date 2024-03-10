// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Actors/BasePickupItem.h"


// Sets default values
ABasePickupItem::ABasePickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UBaseInventoryItem* ABasePickupItem::GetInventoryItem()
{
	if (InventoryItem != nullptr)
	{
		UBaseInventoryItem* InventoryItemInstance = NewObject<UBaseInventoryItem>(this,InventoryItem.Get());
		if (InventoryItemInstance != nullptr)
		{
			return InventoryItemInstance;
		}
		UE_LOG(LogTemp, Error, TEXT("Failed To Create Object For Inventoryitem"));
		return nullptr;
	}

	UE_LOG(LogTemp, Error, TEXT("Inventory Item Is Set To Null"));
	return nullptr;
}

void ABasePickupItem::MarkInventoryItemForPendingKill()
{
	if (UBaseInventoryItem* Item = GetInventoryItem())
	{
		Item->MarkAsGarbage();
	}
}

// Called when the game starts or when spawned
void ABasePickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

