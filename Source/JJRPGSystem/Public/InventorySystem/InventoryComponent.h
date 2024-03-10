// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BasePickupItem.h"
#include "Components/ActorComponent.h"
#include "Data/Inventory/Structures/FInventoryInfo.h"

#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JJRPGSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintPure)
	float GetMaxWeight() const {return MaxWeight;}

	UFUNCTION(BlueprintPure)
	float GetCurrentWeight() const {return CurrentWeight;}

	UFUNCTION(BlueprintPure)
	float GetCurrentSellMultiplier() const {return CurrentSellMultiplier;}

	UFUNCTION(BlueprintPure)
	bool GetIsEncumbered() const {return IsEncumbered;}

	UFUNCTION(BlueprintPure)
	float GetDropDistance() const {return DropDistance;}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMaxWeight(const float NewMaxWeight){MaxWeight = NewMaxWeight;}

	UFUNCTION(BlueprintCallable)
	void SetCurrentWeight(const float NewCurrentWeight);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSellMultiplier(const float NewSellMultiplier);

	UFUNCTION(BlueprintCallable)
	void SetIsEncumbered(const bool NewIsEncumbered) {IsEncumbered = NewIsEncumbered;}

	UFUNCTION(BlueprintCallable)
	void SetDropDistance(const float NewDropDistance) {DropDistance = NewDropDistance;}

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool AddItem(ABasePickupItem* ItemRef);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UpdateWeight();

	UFUNCTION(BlueprintCallable)
	void FuckingDEBUG()
	{
		for (int32 i = 0; i < Content.Num(); i++)
		{
			FString Message = FString::Printf(TEXT("Item Class %s, Index %d, Quantity: %d, Weight: %f"),*Content[i].Item->GetName(),i,Content[i].Quantity,GetCurrentWeight());
			GEngine->AddOnScreenDebugMessage(i,GetWorld()->GetDeltaSeconds(),FColor::Blue,Message);
		}
	}

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool DropItem(int32 Index, int32 AmountToDrop);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Content")
	TArray<FInventoryInfo> Content;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Content")
	float DropDistance = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Encumberance", meta = (Setter = "SetMaxWeight", Getter = "GetMaxWeight"))
	float MaxWeight = 350.0f;

	UPROPERTY(VisibleAnywhere, Category = "Inventory|Encumberance", meta = (Setter = "SetCurrentWeight", Getter = "GetCurrentWeight"))
	float CurrentWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Encumberance", meta = (Setter = "SetIsEncumbered", Getter = "GetIsEncumbered"))
	bool IsEncumbered = false;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Shop", meta = (Setter = "SetCurrentSellMultiplier", Getter = "GetCurrentSellMultiplier"))
	float CurrentSellMultiplier = 1.0f;
	
};
