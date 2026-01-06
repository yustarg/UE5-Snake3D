// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeGameManager.generated.h"

class IGameRule;
class ASnakeAIController;
class ASnakeItemSpawner;
class USnakeGridSubsystem;
class ASnakeItem;
class ASnake;

UCLASS()
class SNAKE3D_API ASnakeGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;
	
	/* ===== 流程接口（由 GameMode 调用） ===== */
	void StartGame();
	void StopGame();

	DECLARE_MULTICAST_DELEGATE(FOnPlayerEatFood, int32);
	FOnPlayerEatFood OnPlayerEatFood;
	
	DECLARE_MULTICAST_DELEGATE(FOnPlayerSnakeDied);
	FOnPlayerSnakeDied OnPlayerSnakeDied;
	
	void StepMove();
	void RegisterSnake(ASnake* InSnake);
	USnakeGridSubsystem* GetGridSubsystem() const { return GridSubsystem; }

private:
	void Initialize();
	void RestartGame();
	void CleanGame();
	
	const float GlobalStepInterval = 0.05f;
	FTimerHandle MoveTimer;
	void StartStepTimer();
	
	void SpawnPlayerSnake();
	void SpawnAISnakes();
	void SpawnItems() const;
	void OnSnakeDied(ASnake* Snake) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	TSubclassOf<ASnake> PlayerSnakeClass;
	UPROPERTY()
	TObjectPtr<ASnake> PlayerSnake;
	
	UPROPERTY(editDefaultsOnly, Category="Snake")
	TSubclassOf<ASnakeAIController> AIControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	int32 NumAI = 0;
	
	UPROPERTY()
	TArray<TObjectPtr<ASnake>> Snakes;
	
	UPROPERTY()
	USnakeGridSubsystem* GridSubsystem;
	
	UPROPERTY(EditInstanceOnly, Category="Item")
	TObjectPtr<ASnakeItemSpawner> ItemSpawner;
	
	UPROPERTY()
	TArray<TScriptInterface<IGameRule>> ActiveRules;
	
};
