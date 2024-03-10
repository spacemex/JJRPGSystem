// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryComponent.h"
#include "Data/Inventory/Structures/FStaticItemInfo.h"
#include "Kismet/GameplayStatics.h"


struct FStaticItemInfo;
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::SetCurrentWeight(const float NewCurrentWeight)
{
	CurrentWeight += NewCurrentWeight;
	if (CurrentWeight < 0 )
	{
		CurrentWeight = 0;
	}
}

void UInventoryComponent::SetCurrentSellMultiplier(const float NewSellMultiplier)
{
	CurrentSellMultiplier += NewSellMultiplier;
	if (CurrentSellMultiplier < 1.0f)
	{
		CurrentSellMultiplier = 1.0f;
	}
}

bool UInventoryComponent::DropItem_Implementation(int32 Index, int32 AmountToDrop)
{
	if (Index >= 0 && Index < Content.Num() )
	{
		FInventoryInfo ItemData = Content[Index];
		TSubclassOf<UBaseInventoryItem> LocalItem = ItemData.Item;
		
		if (ItemData.Quantity > AmountToDrop)
		{
			ItemData.Quantity -= AmountToDrop;
		}
		else
		{
			AmountToDrop = ItemData.Quantity;
			Content.RemoveAt(Index);
		}
		UpdateWeight();
		if (LocalItem != nullptr)
		{
			if (const UBaseInventoryItem* InventoryItemInstance = NewObject<UBaseInventoryItem>(this,LocalItem))
			{
				if (!InventoryItemInstance->ItemInfo.IsNull())
				{
					if (const FStaticItemInfo* Row = InventoryItemInstance->ItemInfo.GetRow<FStaticItemInfo>(TEXT("Lookup")))
					{
						if (UWorld* World = GetWorld())
						{
							FActorSpawnParameters SpawnParameters;

							SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

							FVector StartTrace = World->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

							FVector Direction = World->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();

							FVector EndTrace = StartTrace + Direction * GetDropDistance();

							FTransform SpawnTransform = FTransform(Direction.Rotation(), EndTrace);


							FHitResult HitResult;
							FCollisionQueryParams CollisionParams;

							if (World->LineTraceSingleByChannel(HitResult,StartTrace,EndTrace,ECC_Visibility,CollisionParams))
							{
								UE_LOG(LogTemp, Warning, TEXT("Actor location: %s"), *HitResult.Location.ToString());

								SpawnTransform.SetLocation(HitResult.Location);
								if (ABasePickupItem* SpawnedItem = World->SpawnActor<ABasePickupItem>(Row->PickupRef,SpawnTransform,SpawnParameters))
								{
									UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully"));

									SpawnedItem->SetQuantity(AmountToDrop);
									SpawnedItem->SetInventoryItem(LocalItem);
									
								}
								return true;
							}
						}
					}
				}
			}
			UE_LOG(LogTemp,Error,TEXT("WTF"));
		}
	}
	return false;
}

void UInventoryComponent::UpdateWeight_Implementation()
{
	float LocalWeight = 0.0f;
	CurrentWeight = 0.0f;
	for (int32 i = 0; i < Content.Num(); i++)
	{
		if (Content[i].Item != nullptr)
		{
			if (UBaseInventoryItem* InventoryItemInstance = NewObject<UBaseInventoryItem>(this,Content[i].Item))
			{
				if (!InventoryItemInstance->ItemInfo.IsNull())
				{
					if (const FStaticItemInfo* Row = InventoryItemInstance->ItemInfo.GetRow<FStaticItemInfo>(TEXT("Lookup")))
					{
						LocalWeight += (Content[i].Quantity * Row->ItemWeight);
					}
				}
				if (InventoryItemInstance)
				{
					InventoryItemInstance->ConditionalBeginDestroy();
				}
			}
		}
	}
	SetCurrentWeight(LocalWeight);
	if (GetCurrentWeight() > GetMaxWeight())
	{
		SetIsEncumbered(true);
	}
	else
	{
		SetIsEncumbered(false);
	}
}

bool UInventoryComponent::AddItem_Implementation(ABasePickupItem* ItemRef)
{
	if (ItemRef != nullptr)
	{
		if (ABasePickupItem* Item = Cast<ABasePickupItem>(ItemRef))
		{
			bool bFound = false;
			for (int32 i = 0; i < Content.Num(); i++)
			{
				if (Content[i].Item == Item->GetInventoryItem()->GetClass())
				{
					Content[i].Quantity += Item->GetQuantity();
					Item->MarkInventoryItemForPendingKill();
					UpdateWeight();
					bFound = true;
					break;
				}
			}
			if (!bFound)
			{
				if (const UBaseInventoryItem* InventoryItem = Item->GetInventoryItem())
				{
					FInventoryInfo NewInventoryInfo;
					NewInventoryInfo.Item = InventoryItem->GetClass();
					NewInventoryInfo.Quantity = Item->GetQuantity();
					Content.Add(NewInventoryInfo);
					Item->MarkInventoryItemForPendingKill();
					
					UpdateWeight();
					return true;
				}
				UE_LOG(LogTemp, Error, TEXT("Failed To Get Inventory Item"))
				return false; //Failed to Add New Item
			}
			return true; // Item Was Found And Quantity Was Increased 
		}
		UE_LOG(LogTemp,Error,TEXT("Failed To Cast To ItemRef"));
		return false; //Failed To Cast To The ItemPickup 
	}
	UE_LOG(LogTemp,Error,TEXT("Failed To add Item Invalid ItemRef"))
	return false; //Failed Item Ref Is Invalid
}

