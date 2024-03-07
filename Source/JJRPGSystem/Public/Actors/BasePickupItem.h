// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GetPickupItemReference.h"
#include "BasePickupItem.generated.h"

class ABaseInventoryItem;

UCLASS()
class JJRPGSYSTEM_API ABasePickupItem : public AActor, public IGetPickupItemReference
{
	GENERATED_BODY()

	virtual ABasePickupItem* GetPickupItemReference_Implementation() override;

public:
	// Sets default values for this actor's properties
	ABasePickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseInventoryItem> InventoryItem = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Quantity = 1;
	
};
