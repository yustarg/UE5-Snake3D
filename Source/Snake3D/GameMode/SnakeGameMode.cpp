// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

#include "EngineUtils.h"
#include "Snake3D/SnakeGameManager.h"
#include "Snake3D/GameState/SnakeGameState.h"

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
