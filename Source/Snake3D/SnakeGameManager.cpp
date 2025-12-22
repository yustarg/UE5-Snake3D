// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameManager.h"

// Sets default values
ASnakeGameManager::ASnakeGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASnakeGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	BodyGrids = {
		FIntPoint(-1,0),
		FIntPoint(-2,0),
	}; 
	
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &ASnakeGameManager::StepMove, 0.25f, true);
}

// Called every frame
void ASnakeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeGameManager::SetDirection(ESnakeDirection NewDirection)
{
	Direction = NewDirection;
}

FVector ASnakeGameManager::GridToWorld(const FIntPoint& GridPoint) const
{
	return FVector(GridPoint.X * GridSize, GridPoint.Y * GridSize, 0);
}

void ASnakeGameManager::StepMove()
{
	FIntPoint NewHeadGrid(0, 0);
	switch (Direction)
	{
		case ESnakeDirection::Left:
			NewHeadGrid = HeadGrid + FIntPoint(0, -1);
			break;
		case ESnakeDirection::Right:
			NewHeadGrid = HeadGrid + FIntPoint(0, 1);
			break;
		case ESnakeDirection::Up:
			NewHeadGrid = HeadGrid + FIntPoint(1, 0);
			break;
		case ESnakeDirection::Down:
			NewHeadGrid = HeadGrid + FIntPoint(-1, 0);
			break;
	}
	
	if ((NewHeadGrid.X < GridMin || NewHeadGrid.X > GridMax) || (NewHeadGrid.Y < GridMin || NewHeadGrid.Y > GridMax))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Wall"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		return;
	}
	
	if (BodyGrids.Contains(NewHeadGrid))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Self"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		return;
	}
	
	BodyGrids.Insert(HeadGrid, 0);
	HeadGrid = NewHeadGrid;

	if (HeadGrid == FoodGrid)
	{
		// TODO spawn food
	}
	else
	{
		BodyGrids.Pop();
	}
}

