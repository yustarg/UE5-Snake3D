// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SnakeGridSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API USnakeGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	FIntPoint WorldToGrid(const FVector& WorldPos) const;
	FVector GridToWorld(const FIntPoint& GridPoint) const;

	float GetGridSize() const { return GridSize; }
	FIntPoint GetGridMin() const { return GridMin; }
	FIntPoint GetGridMax() const { return GridMax; }
	
	void RebuildDynamicOccupied(const TArray<FIntPoint>& Cells);
	bool IsInside(const FIntPoint& Cell) const;
	bool IsStaticBlocked(const FIntPoint& Cell) const;
	bool IsDynamicOccupied(const FIntPoint& Cell) const;
	bool IsOccupied(const FIntPoint& Cell) const;
	
	void RegisterItemCell(const FIntPoint& Cell);
	void UnregisterItemCell(const FIntPoint& Cell);
	bool IsCellFreeForItem(const FIntPoint& Cell) const;
	FIntPoint GetRandomFreeCellForItem() const;

	
private:
	void InitializeGrid(int32 InGridSize, FIntPoint InMin, FIntPoint InMax, FVector InGridOrigin);
	
	float GridSize = 100.f;
	FIntPoint GridMin = -20;
	FIntPoint GridMax = 20;
	FVector GridOrigin = FVector::ZeroVector;
	
	TSet<FIntPoint> StaticBlocked;
	TSet<FIntPoint> DynamicBlocked;
	TSet<FIntPoint> ItemOccupied;
};
