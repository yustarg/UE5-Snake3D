// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeGameManager.generated.h"

class USnakeGridSubsystem;
class ASnakeItem;
class ASnake;

UENUM(BlueprintType)
enum class ESnakeGameState : uint8
{
	Playing,
	GameOver
};

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
	void StepMove();
	void RestartGame();
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, ESnakeGameState)
	FOnGameStateChanged OnGameStateChanged;
	
	UPROPERTY()
	TObjectPtr<ASnake> PlayerSnake;
	
	ESnakeGameState GameState = ESnakeGameState::Playing;

private:
	void SetGameState(ESnakeGameState NewState);
	void SpawnFood();
	
	FTimerHandle MoveTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	TSubclassOf<ASnake> PlayerSnakeClass;
	
	UPROPERTY()
	TObjectPtr<ASnakeItem> ItemFood;
	
	UPROPERTY(EditDefaultsOnly, Category="Food");
	TSubclassOf<ASnakeItem> FoodActorClass;
	
	UPROPERTY()
	USnakeGridSubsystem* GridSubsystem;
};
