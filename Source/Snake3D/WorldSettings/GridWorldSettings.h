// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "GridWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API AGridWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	AGridWorldSettings();
	
	// 单个格子的世界尺寸
	UPROPERTY(EditAnywhere, Category="Grid")
	int32 GridSize = 100;

	// 最小格子坐标
	UPROPERTY(EditAnywhere, Category="Grid")
	FIntPoint GridMin = FIntPoint(-20, -20);

	// 最大格子坐标
	UPROPERTY(EditAnywhere, Category="Grid")
	FIntPoint GridMax = FIntPoint(20, 20);
	
	UPROPERTY(EditAnywhere, Category="Grid")
	FVector GridOrigin = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category="Grid|Occupancy")
	TArray<FIntPoint> StaticBlockedCells;
};
