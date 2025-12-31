// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SnakeGameState.generated.h"

UENUM(BlueprintType)
enum class ESnakeMatchState : uint8
{
	Waiting,
	Playing,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(
    FOnMatchStateChanged,
    ESnakeMatchState
);

UCLASS()
class SNAKE3D_API ASnakeGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ESnakeMatchState GetMatchState() const { return MatchState; }
	void SetMatchState(ESnakeMatchState NewState);
	FOnMatchStateChanged OnMatchStateChanged;
	
protected:
	ESnakeMatchState MatchState = ESnakeMatchState::Waiting;
};
