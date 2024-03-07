// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BasePickupItem.h"


ABasePickupItem* ABasePickupItem::GetPickupItemReference_Implementation()
{
	return this;
}

// Sets default values
ABasePickupItem::ABasePickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

