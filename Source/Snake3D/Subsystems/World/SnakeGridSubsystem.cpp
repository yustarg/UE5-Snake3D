// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGridSubsystem.h"

#include "Snake3D/WorldSettings/GridWorldSettings.h"

void USnakeGridSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	if (const auto* Setting = Cast<AGridWorldSettings>(InWorld.GetWorldSettings()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("GridMin: %s, GridMax: %s"),
			*Setting->GridMin.ToString(),
			*Setting->GridMax.ToString()
		);

		InitializeGrid(Setting->GridSize, Setting->GridMin, Setting->GridMax, Setting->GridOrigin);
		
		StaticBlocked.Empty();
		for (auto Cell : Setting->StaticBlockedCells)
		{
			if (IsInside(Cell))
			{
				StaticBlocked.Add(Cell);
			}
		}
		ItemOccupied.Empty();
		DynamicBlocked.Empty();
	}
}

void USnakeGridSubsystem::InitializeGrid(const int32 InGridSize, const FIntPoint InMin, 
	const FIntPoint InMax, const FVector& InGridOrigin)
{
	GridSize = InGridSize;
	GridMin = InMin;
	GridMax = InMax;
	GridOrigin = InGridOrigin;
}

FIntPoint USnakeGridSubsystem::WorldToGrid(const FVector& WorldPos) const
{
	return FIntPoint(
		FMath::RoundToInt32((WorldPos.X - GridOrigin.X) / GridSize), 
		FMath::RoundToInt32((WorldPos.Y - GridOrigin.Y) / GridSize));
}

FVector USnakeGridSubsystem::GridToWorld(const FIntPoint& GridPoint) const
{
	return GridOrigin + FVector(GridPoint.X * GridSize, GridPoint.Y * GridSize, 0);
}

void USnakeGridSubsystem::RebuildDynamicOccupied()
{
	DynamicBlocked.Empty();
}

void USnakeGridSubsystem::UpdateDynamicOccupied(const TArray<FIntPoint>& Cells)
{
	for (auto Cell : Cells)
	{
		DynamicBlocked.Add(Cell);
	}
}

bool USnakeGridSubsystem::IsInside(const FIntPoint& Cell) const
{
	return Cell.X >= GridMin.X && Cell.X <= GridMax.X && 
		Cell.Y >= GridMin.Y && Cell.Y <= GridMax.Y;
}

bool USnakeGridSubsystem::IsStaticBlocked(const FIntPoint& Cell) const
{
	return StaticBlocked.Contains(Cell);
}

bool USnakeGridSubsystem::IsDynamicOccupied(const FIntPoint& Cell) const
{
	return DynamicBlocked.Contains(Cell);
}

bool USnakeGridSubsystem::IsOccupied(const FIntPoint& Cell) const
{
	return IsStaticBlocked(Cell) || IsDynamicOccupied(Cell);
}

FIntPoint USnakeGridSubsystem::GetRandomFreeCell() const
{
	TArray<FIntPoint> FreeCells;

	for (int i = GridMin.X; i <= GridMax.X; ++i)
	{
		for (int j = GridMin.Y; j <= GridMax.Y; ++j)
		{
			FIntPoint GridPoint(i, j);
			if (IsCellFreeForItem(GridPoint))
			{
				FreeCells.Add(GridPoint);
			}
		}
	}
	
	check(FreeCells.Num() > 0);
	
	const int RandomIndex = FMath::RandRange(0, FreeCells.Num() - 1);
	return FreeCells[RandomIndex];
}

void USnakeGridSubsystem::Clear()
{
	StaticBlocked.Empty();
	DynamicBlocked.Empty();
	ItemOccupied.Empty();
}

void USnakeGridSubsystem::RegisterItemCell(const FIntPoint& Cell)
{
	ItemOccupied.Add(Cell);
}

void USnakeGridSubsystem::UnregisterItemCell(const FIntPoint& Cell)
{
	ItemOccupied.Remove(Cell);
}

bool USnakeGridSubsystem::IsCellFreeForItem(const FIntPoint& Cell) const
{
	return IsInside(Cell)
		&& !StaticBlocked.Contains(Cell)
		&& !DynamicBlocked.Contains(Cell)
		&& !ItemOccupied.Contains(Cell);
}
