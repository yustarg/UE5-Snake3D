// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameManager.h"

#include "SnakeSegment.h"

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
	
	SpawnInitialSegments();
	
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &ASnakeGameManager::StepMove, 0.25f, true);
	
	SpawnFood();
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
	PrevHeadGrid = HeadGrid;
	
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
		SetGameState(ESnakeGameState::GameOver);
		return;
	}
	
	if (BodyGrids.Contains(NewHeadGrid))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Self"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		SetGameState(ESnakeGameState::GameOver);
		return;
	}
	
	BodyGrids.Insert(HeadGrid, 0);
	HeadGrid = NewHeadGrid;

	if (HeadGrid == FoodGrid)
	{
		SpawnFood();
	}
	else
	{
		BodyGrids.Pop();
	}
	
	SyncSegments();
}

void ASnakeGameManager::SetGameState(const ESnakeGameState NewState)
{
	if (GameState == NewState) return;
	
	GameState = NewState;
	OnGameStateChanged.Broadcast(NewState);
}

void ASnakeGameManager::SpawnInitialSegments()
{
	if (!SnakeSegmentClass) return;

	for (auto Seg : Segments)
	{
		if (Seg) Seg->Destroy();
	}
	Segments.Empty();
	
	FVector HeadPos = GridToWorld(HeadGrid);
	AActor* HeadSeg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, HeadPos, FRotator::ZeroRotator);
	Segments.Add(HeadSeg);
	
	for (const FIntPoint& BodyGrid : BodyGrids)
	{
		FVector BodyPos = GridToWorld(BodyGrid);
		AActor* BodySeg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, BodyPos, FRotator::ZeroRotator);
		Segments.Add(BodySeg);
	}
}

void ASnakeGameManager::SyncSegments()
{
	const int32 SegmentsNeeded = BodyGrids.Num() + 1;

	while (Segments.Num() < SegmentsNeeded)
	{
		if (!SnakeSegmentClass) break;
		
		AActor* Seg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, 
			FVector::Zero(), FRotator::ZeroRotator);
		Segments.Add(Seg);
	}
	
	while (Segments.Num() > SegmentsNeeded)
	{
		if (AActor* Seg = Segments.Last())
		{
			Seg->Destroy();
		}
		Segments.Pop();
	}
	
	// Head
	if (Segments.Num() > 0)
	{
		ASnakeSegment* Head = Cast<ASnakeSegment>(Segments[0]);
		Head->SetSegmentType(ESnakeSegmentType::Head);
		Head->SetFacingDirection(HeadGrid - PrevHeadGrid);
		Head->SetActorLocation(GridToWorld(HeadGrid));
	}
	
	// Body
	for (int i = 0; i < BodyGrids.Num(); ++i)
	{
		const int SegIndex = i + 1;

		if (ASnakeSegment* Body = Cast<ASnakeSegment>(Segments[SegIndex]))
		{
			Body->SetSegmentType(ESnakeSegmentType::Body);
			Body->SetActorLocation(GridToWorld(BodyGrids[i]));
		}
	}
}

void ASnakeGameManager::SpawnFood()
{
	if (!FoodActorClass) return;
	
	TArray<FIntPoint> FreeGrids;

	for (int i = GridMin; i <= GridMax; ++i)
	{
		for (int j = GridMin; j <= GridMax; ++j)
		{
			FIntPoint GridPoint(i, j);
			if (!IsGridOccupied(GridPoint))
			{
				FreeGrids.Add(GridPoint);
			}
		}
	}
	
	if (FreeGrids.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No space left for food."));
		return;
	}
	
	const int RandomIndex = FMath::RandRange(0, FreeGrids.Num() - 1);
	FoodGrid = FreeGrids[RandomIndex];
	const FVector RandomPos = GridToWorld(FoodGrid);
	
	if (FoodActor)
	{
		FoodActor->SetActorLocation(RandomPos);
	}
	else
	{
		FoodActor = GetWorld()->SpawnActor<AActor>(FoodActorClass, RandomPos, FRotator::ZeroRotator);
	}
}

bool ASnakeGameManager::IsGridOccupied(const FIntPoint& GridPoint) const
{
	if (GridPoint == HeadGrid) return true;
	
	return BodyGrids.Contains(GridPoint);
}

void ASnakeGameManager::RestartGame()
{
	for (AActor* Seg : Segments)
	{
		if (Seg) Seg->Destroy();
	}
	Segments.Empty();
	
	if (FoodActor)
	{
		FoodActor->Destroy();
		FoodActor = nullptr;
	}
	
	HeadGrid = FIntPoint(0, 0);
	BodyGrids = {
		FIntPoint(-1, 0),
		FIntPoint(-2, 0)
	};
	PrevHeadGrid = HeadGrid;
	
	SpawnInitialSegments();
	SpawnFood();
	
	SetGameState(ESnakeGameState::Playing);
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this,
		&ASnakeGameManager::StepMove,
		0.25f,
		true);
}

