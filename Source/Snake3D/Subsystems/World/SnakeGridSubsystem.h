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
	void InitializeGrid(int32 InGridSize, FIntPoint InMin, FIntPoint InMax);
	
	FIntPoint WorldToGrid(const FVector& WorldPos) const;
	FVector GridToWorld(const FIntPoint& GridPoint) const;
	bool IsInside(const FIntPoint& Grid) const;

	float GetGridSize() const { return GridSize; }
	FIntPoint GetGridMin() const { return GridMin; }
	FIntPoint GetGridMax() const { return GridMax; }
	
private:
	float GridSize = 100.f;
	FIntPoint GridMin = -20;
	FIntPoint GridMax = 20;
};
