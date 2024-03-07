// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GetInventoryItemReference.h"
#include "BaseInventoryItem.generated.h"

UCLASS()
class JJRPGSYSTEM_API ABaseInventoryItem : public AActor, public IGetInventoryItemReference
{
	GENERATED_BODY()

	virtual ABaseInventoryItem* GetInventoryItemReference_Implementation() override;
public:
	// Sets default values for this actor's properties
	ABaseInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	FName RowName;
	
};
