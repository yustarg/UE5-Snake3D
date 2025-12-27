// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameManager.h"
#include "Snake.h"
#include "Items/SnakeItem.h"
#include "Items/SnakeItemSpawner.h"
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
	
	StartStepTimer();
	
	ItemSpawner->SpawnFood();
}

// Called every frame
void ASnakeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeGameManager::StepMove()
{
	if (!PlayerSnake) return;
	
	GridSubsystem->RebuildDynamicOccupied(PlayerSnake->GetHeadAndBody());
	
	PlayerSnake->StepMove();
	
	if (!GridSubsystem->IsInside(PlayerSnake->GetHead()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Wall"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		SetGameState(ESnakeGameState::GameOver);
		return;
	}
	
	if (GridSubsystem->IsOccupied(PlayerSnake->GetHead()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Self or Obstacles"));
		GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
		SetGameState(ESnakeGameState::GameOver);
		return;
	}

	if (PlayerSnake->GetHead() == ItemSpawner->CurrentFood->GetGrid())
	{
		if (PlayerSnake->ApplyEffect(ItemSpawner->CurrentFood->GetEffect()))
		{
			RestartStepTimerIfNeeded();
		}
		ItemSpawner->SpawnFood();
	}
	
	PlayerSnake->SyncSegments();
}

void ASnakeGameManager::SetGameState(const ESnakeGameState NewState)
{
	if (GameState == NewState) return;
	
	GameState = NewState;
	OnGameStateChanged.Broadcast(NewState);
}

void ASnakeGameManager::StartStepTimer()
{
	UWorld* World = GetWorld();
	if (!World || !PlayerSnake) return;

	const float Interval = PlayerSnake->GetMoveInterval();

	World->GetTimerManager().SetTimer(
		MoveTimer,
		this,
		&ASnakeGameManager::StepMove,
		Interval,
		true
	);
}

void ASnakeGameManager::RestartStepTimerIfNeeded()
{
	UWorld* World = GetWorld();
	if (!World || !PlayerSnake) return;

	const float Interval = PlayerSnake->GetMoveInterval();

	World->GetTimerManager().ClearTimer(MoveTimer);
	World->GetTimerManager().SetTimer(
		MoveTimer,
		this,
		&ASnakeGameManager::StepMove,
		Interval,
		true
	);
}

void ASnakeGameManager::RestartGame()
{
	PlayerSnake->Restart();
	ItemSpawner->RemoveItem(ItemSpawner->CurrentFood);
	ItemSpawner->SpawnFood();
	SetGameState(ESnakeGameState::Playing);
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this,
		&ASnakeGameManager::StepMove,
		0.25f,
		true);
}

