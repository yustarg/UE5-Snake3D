// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameManager.h"
#include "Snake.h"
#include "Items/SnakeFood.h"
#include "Items/SnakeItem.h"
#include "Subsystems/World/SnakeGridSubsystem.h"

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
	
	GridSubsystem = GetWorld()->GetSubsystem<USnakeGridSubsystem>();
	
	PlayerSnake = GetWorld()->SpawnActor<ASnake>(PlayerSnakeClass);
	PlayerSnake->Initialize();
	
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &ASnakeGameManager::StepMove, 0.25f, true);
	
	SpawnFood();
}

// Called every frame
void ASnakeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeGameManager::StepMove()
{
	if (!PlayerSnake) return;
	
	PlayerSnake->StepMove();
	
	if (!GridSubsystem->IsInside(PlayerSnake->GetHead()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Wall"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		SetGameState(ESnakeGameState::GameOver);
		return;
	}
	
	if (PlayerSnake->GetBody().Contains(PlayerSnake->GetHead()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Self"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		SetGameState(ESnakeGameState::GameOver);
		return;
	}

	if (PlayerSnake->GetHead() == ItemFood->Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item class: %s"),  *ItemFood->GetClass()->GetName());
		ItemFood->ApplyTo(PlayerSnake);
		SpawnFood();
	}
	
	PlayerSnake->SyncSegments();
}

void ASnakeGameManager::SetGameState(const ESnakeGameState NewState)
{
	if (GameState == NewState) return;
	
	GameState = NewState;
	OnGameStateChanged.Broadcast(NewState);
}

void ASnakeGameManager::SpawnFood()
{
	if (!FoodActorClass) return;
	
	TArray<FIntPoint> FreeGrids;

	const FIntPoint GridMin = GridSubsystem->GetGridMin();
	const FIntPoint GridMax = GridSubsystem->GetGridMax();
	
	for (int i = GridMin.X; i <= GridMax.X; ++i)
	{
		for (int j = GridMin.Y; j <= GridMax.Y; ++j)
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
	const FIntPoint FoodGrid = FreeGrids[RandomIndex];
	const FVector RandomPos = GridSubsystem->GridToWorld(FoodGrid);
	
	if (ItemFood)
	{
		ItemFood->SetActorLocation(RandomPos);
	}
	else
	{
		ItemFood = GetWorld()->SpawnActor<ASnakeFood>(FoodActorClass, RandomPos, FRotator::ZeroRotator);
	}
	
	ItemFood->Grid = FoodGrid;
}

bool ASnakeGameManager::IsGridOccupied(const FIntPoint& GridPoint) const
{
	if (GridPoint == PlayerSnake->GetHead()) return true;
	
	return PlayerSnake->GetBody().Contains(GridPoint);
}

void ASnakeGameManager::RestartGame()
{
	PlayerSnake->Restart();
	if (ItemFood)
	{
		ItemFood->Destroy();
		ItemFood = nullptr;
	}
	
	SpawnFood();
	SetGameState(ESnakeGameState::Playing);
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this,
		&ASnakeGameManager::StepMove,
		0.25f,
		true);
}

