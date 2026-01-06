// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerState.h"

void ASnakePlayerState::AddScore(const int32 Delta)
{
	SetScore(GetScore() + Delta);
}
