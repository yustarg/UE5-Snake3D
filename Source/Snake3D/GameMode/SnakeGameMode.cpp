// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

#include "EngineUtils.h"
#include "Snake3D/Snake.h"
#include "Snake3D/SnakeGameManager.h"
#include "Snake3D/GameState/SnakeGameState.h"
#include "Snake3D/Items/ItemFood.h"
#include "Snake3D/Items/SnakeItem.h"
#include "Snake3D/Items/SnakeItemSpawner.h"
#include "Snake3D/PlayerState/SnakePlayerState.h"

ASnakeGameMode::ASnakeGameMode()
{
	PlayerStateClass = ASnakePlayerState::StaticClass();
}

void ASnakeGameMode::SetMatchState(const ESnakeMatchState NewState) const
{
	ASnakeGameState* GS = GetGameState<ASnakeGameState>();
	
	if (!GS || GS->GetMatchState() == NewState) return;

	GS->SetMatchState(NewState);

	switch (NewState)
	{
	case ESnakeMatchState::Waiting:
		OnEnterWaiting();
		break;
	case ESnakeMatchState::Playing:
		OnEnterPlaying();
		break;
	case ESnakeMatchState::GameOver:
		OnEnterGameOver();
		break;
	}
}

void ASnakeGameMode::RequestRestart()
{
	ASnakeGameState* GS = GetGameState<ASnakeGameState>();
	if (GS && GS->GetMatchState() == ESnakeMatchState::GameOver)
	{
		return;
	}
	
	SetMatchState(ESnakeMatchState::Waiting);
	ResetLevel();
	SetMatchState(ESnakeMatchState::Playing);
}

void ASnakeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 找 GameManager（推荐 Tag 或 WorldSubsystem）
	for (TActorIterator<ASnakeGameManager> It(GetWorld()); It; ++It)
	{
		GameManager = *It;
		break;
	}
	
	check(GameManager);
	
	// 2. 监听 GameManager 事件
	GameManager->OnPlayerSnakeDied.AddUObject(
		this,
		&ASnakeGameMode::OnEnterGameOver
	);

	ItemSpawner = GameManager->GetItemSpawner();
	ItemSpawner->OnItemSpawned.AddUObject(this, &ASnakeGameMode::HandleItemSpawned);
	
	// 3. 初始状态
	SetMatchState(ESnakeMatchState::Playing);
}

void ASnakeGameMode::OnEnterWaiting() const
{
	// TODO
}

void ASnakeGameMode::OnEnterPlaying() const
{
	GameManager->StartGame();
}

void ASnakeGameMode::OnEnterGameOver() const
{
	GameManager->StopGame();
}

ASnakePlayerState* ASnakeGameMode::CreatePlayerState() const
{
	ASnakePlayerState* PS =
		GetWorld()->SpawnActor<ASnakePlayerState>(PlayerStateClass);

	PS->SetPlayerName(TEXT("Snake Player State"));
	return PS;
}

void ASnakeGameMode::HandleItemSpawned(ASnakeItem* Item)
{
	if (!Item) return;

	Item->OnItemConsumed.AddUObject(
		this,
		&ASnakeGameMode::HandleItemConsumed
	);
}

void ASnakeGameMode::HandleItemConsumed(ASnake* Snake, ASnakeItem* Item)
{
	if (Item->IsA<AItemFood>())
	{
		AddScoreForSnake(Snake, 10);
	}
}

void ASnakeGameMode::AddScoreForSnake(const ASnake* Snake, const int32 Score)
{
	if (!Snake) return;

	Snake->OwningPlayerState->AddScore(Score);
}
