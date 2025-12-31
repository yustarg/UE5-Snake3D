// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameState.h"

void ASnakeGameState::SetMatchState(ESnakeMatchState NewState)
{
	if (NewState == MatchState) return;
	
	MatchState = NewState;
	OnMatchStateChanged.Broadcast(MatchState);
}
