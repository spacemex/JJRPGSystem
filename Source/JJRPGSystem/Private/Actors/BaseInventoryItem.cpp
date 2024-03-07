// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseInventoryItem.h"


ABaseInventoryItem* ABaseInventoryItem::GetInventoryItemReference_Implementation()
{
	return this;
}

// Sets default values
ABaseInventoryItem::ABaseInventoryItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

