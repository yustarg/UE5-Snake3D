// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBoardVisualizer.h"

#include "Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
AGridBoardVisualizer::AGridBoardVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridBoardVisualizer::BeginPlay()
{
	Super::BeginPlay();
	
	GridSubsystem = GetWorld()->GetSubsystem<USnakeGridSubsystem>();
	DrawDebugGrid();
}

// Called every frame
void AGridBoardVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridBoardVisualizer::DrawDebugGrid() const
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	const FIntPoint GridMin = GridSubsystem->GetGridMin();
	const FIntPoint GridMax = GridSubsystem->GetGridMax();
	const float GridSize = GridSubsystem->GetGridSize();
	
	const float Half = GridSize * 0.5f;
	const float ZOffset = 5.f;

	for (int32 X = GridMin.X; X <= GridMax.X; ++X)
	{
		for (int32 Y = GridMin.Y; Y <= GridMax.Y; ++Y)
		{
			const FIntPoint Cell(X, Y);
			const FVector Center =
				GridSubsystem->GridToWorld(Cell) + FVector(0, 0, ZOffset);

			DrawDebugBox(
				World,
				Center,
				FVector(Half, Half, 2.f), // 薄盒子
				FColor::Green,
				true,   // Persistent
				-1.f,   // Lifetime
				0,
				1.5f    // 线宽（屏幕空间）
			);
		}
	}
}

