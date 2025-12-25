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

	const float Z = 5.f;
	const FColor Color = FColor::Green;
	const FIntPoint GridMin = GridSubsystem->GetGridMin();
	const FIntPoint GridMax = GridSubsystem->GetGridMax();
	const float GridSize = GridSubsystem->GetGridSize();
	
	// 画纵线（X 方向）
	for (int32 X = GridMin.X; X <= GridMax.X + 1; ++X)
	{
		FVector Start(
			X * GridSize,
			GridMin.Y * GridSize,
			Z
		);

		FVector End(
			X * GridSize,
			(GridMax.Y + 1) * GridSize,
			Z
		);

		DrawDebugLine(World, Start, End, Color, true, -1.f, 0, 2.f);
	}

	// 画横线（Y 方向）
	for (int32 Y = GridMin.Y; Y <= GridMax.Y + 1; ++Y)
	{
		FVector Start(
			GridMin.X * GridSize,
			Y * GridSize,
			Z
		);

		FVector End(
			(GridMax.X + 1) * GridSize,
			Y * GridSize,
			Z
		);

		DrawDebugLine(World, Start, End, Color, true, -1.f, 0, 2.f);
	}
}

