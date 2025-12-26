// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGridSubsystem.h"

#include "Snake3D/WorldSettings/GridWorldSettings.h"

void USnakeGridSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	if (const auto* Setting = Cast<AGridWorldSettings>(InWorld.GetWorldSettings()))
	{
		InitializeGrid(Setting->GridSize, Setting->GridMin, Setting->GridMax);
	}
}

void USnakeGridSubsystem::InitializeGrid(int32 InGridSize, FIntPoint InMin, FIntPoint InMax)
{
	GridSize = InGridSize;
	GridMin = InMin;
	GridMax = InMax;
}

FIntPoint USnakeGridSubsystem::WorldToGrid(const FVector& WorldPos) const
{
	return FIntPoint(
		FMath::RoundToInt32(WorldPos.X / GridSize), 
		FMath::RoundToInt32(WorldPos.Y / GridSize));
}

FVector USnakeGridSubsystem::GridToWorld(const FIntPoint& GridPoint) const
{
	return FVector(GridPoint.X * GridSize, GridPoint.Y * GridSize, 0);
}

bool USnakeGridSubsystem::IsInside(const FIntPoint& Grid) const
{
	return Grid.X >= GridMin.X && Grid.X <= GridMax.X && 
		Grid.Y >= GridMin.Y && Grid.Y <= GridMax.Y;
}
