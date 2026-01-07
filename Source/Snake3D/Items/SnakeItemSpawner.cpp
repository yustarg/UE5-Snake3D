// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeItemSpawner.h"
#include "SnakeItem.h"
#include "Snake3D/Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
ASnakeItemSpawner::ASnakeItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnakeItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

// Called every frame
void ASnakeItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeItemSpawner::Initialize()
{
	if (bIsInitialized) return;
	
	if (const UWorld* World = GetWorld())
	{
		GridSystem = World->GetSubsystem<USnakeGridSubsystem>();
	}
	bIsInitialized = true;
}

void ASnakeItemSpawner::SpawnRandomItem()
{
	if (CurrentItem)
	{
		RemoveItem(CurrentItem);
	}

	const TSubclassOf<ASnakeItem> ItemClass = PickRandomItemClass();
	if (!ItemClass) return;

	const FIntPoint Cell = GridSystem->GetRandomFreeCell();

	CurrentItem = GetWorld()->SpawnActor<ASnakeItem>(ItemClass);
	CurrentItem->SetGrid(Cell);
	GridSystem->RegisterItemCell(Cell);
	OnItemSpawned.Broadcast(CurrentItem);
}

void ASnakeItemSpawner::RemoveItem(ASnakeItem* Item)
{
	if (!Item) return;

	GridSystem->UnregisterItemCell(Item->GetGrid());
	Item->Destroy();
	if (Item == CurrentItem)
	{
		CurrentItem = nullptr;
	}
}

TSubclassOf<ASnakeItem> ASnakeItemSpawner::PickRandomItemClass() const
{
	float TotalWeight = 0.f;
	for (const auto& Entry : ItemPool)
	{
		TotalWeight += Entry.Weight;
	}

	float R = FMath::FRandRange(0.f, TotalWeight);

	for (const auto& Entry : ItemPool)
	{
		R -= Entry.Weight;
		if (R <= 0.f)
		{
			return Entry.ItemClass;
		}
	}

	return nullptr;
}

