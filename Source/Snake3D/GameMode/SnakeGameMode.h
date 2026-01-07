// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameMode.generated.h"

class ASnakePlayerState;
class ASnake;
class ASnakeItem;
class ASnakeItemSpawner;
class ASnakeGameManager;
enum class ESnakeMatchState : uint8;
/**
 * 
 */
UCLASS()
class SNAKE3D_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASnakeGameMode();

	ASnakePlayerState* CreatePlayerState() const;
	
	void SetMatchState(ESnakeMatchState NewState) const;
	
	UFUNCTION(BlueprintCallable)
	void RequestRestart();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void OnEnterWaiting() const;
	void OnEnterPlaying() const;
	void OnEnterGameOver() const;

	void HandleItemSpawned(ASnakeItem* Item);
	void HandleItemConsumed(ASnake* Snake, ASnakeItem* Item);
	void AddScoreForSnake(const ASnake* Snake, int32 Score);
	
	UPROPERTY()
	TObjectPtr<ASnakeGameManager> GameManager;

	UPROPERTY()
	TObjectPtr<ASnakeItemSpawner> ItemSpawner;
};
