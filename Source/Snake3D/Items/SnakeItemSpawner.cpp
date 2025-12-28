// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeItemSpawner.h"

#include "ItemFood.h"
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

void ASnakeItemSpawner::SpawnFood()
{
	if (CurrentFood)
	{
		RemoveItem(CurrentFood);
	}

	const FIntPoint Cell = GridSystem->GetRandomFreeCellForItem();
	
	CurrentFood = GetWorld()->SpawnActor<AItemFood>(FoodClass);
	CurrentFood->SetGrid(Cell);
	GridSystem->RegisterItemCell(Cell);
}

void ASnakeItemSpawner::RemoveItem(ASnakeItem* Item)
{
	if (!Item) return;

	GridSystem->UnregisterItemCell(Item->GetGrid());
	Item->Destroy();
	if (Item == CurrentFood)
	{
		CurrentFood = nullptr;
	}
}

