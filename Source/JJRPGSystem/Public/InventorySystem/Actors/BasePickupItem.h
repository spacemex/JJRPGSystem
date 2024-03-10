// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/Objects/BaseInventoryItem.h"
#include "BasePickupItem.generated.h"

UCLASS()
class JJRPGSYSTEM_API ABasePickupItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePickupItem();

	UFUNCTION(BlueprintPure)
	UBaseInventoryItem* GetInventoryItem();

	UFUNCTION(BlueprintCallable)
	void MarkInventoryItemForPendingKill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetQuantity(const int32 NewQuantity){Quantity = NewQuantity;}

	UFUNCTION(BlueprintPure)
	int32 GetQuantity() const {return Quantity;}

	UFUNCTION(BlueprintCallable)
	void SetInventoryItem(TSubclassOf<UBaseInventoryItem> NewInventoryItem){InventoryItem = NewInventoryItem;}

private:
	UPROPERTY(EditDefaultsOnly,Category = "Inventory", meta = (Getter = "GetInventoryItem", Setter = "SetInventoryItem"))
	TSubclassOf<UBaseInventoryItem> InventoryItem;
public:
	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category = "Inventory", meta = (Setter = "SetQuantity", Getter = "GetQuantity"))
	int32 Quantity = 1;
};
