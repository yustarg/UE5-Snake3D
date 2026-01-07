// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeItemSpawner.generated.h"

class USnakeGridSubsystem;
class ASnakeItem;

USTRUCT(BlueprintType)
struct FItemSpawnEntry
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeItem> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	float Weight = 1.f;	
};

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnItemSpawned,
	ASnakeItem*
);

UCLASS()
class SNAKE3D_API ASnakeItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Initialize();
	void SpawnRandomItem();
	void RemoveItem(ASnakeItem* Item);
	
	UPROPERTY()
	TObjectPtr<ASnakeItem> CurrentItem;

	FOnItemSpawned OnItemSpawned;

private:
	UPROPERTY()
	USnakeGridSubsystem* GridSystem;
	
	UPROPERTY(EditDefaultsOnly, Category="Item")
	TArray<FItemSpawnEntry> ItemPool;
	
	bool bIsInitialized = false;
	
	TSubclassOf<ASnakeItem> PickRandomItemClass() const;
};
