// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeGameManager.generated.h"

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
	
	static FVector GridToWorld(const FIntPoint& GridPoint);
	void StepMove();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, ESnakeGameState)
	FOnGameStateChanged OnGameStateChanged;
	
public:
	UPROPERTY()
	TObjectPtr<ASnake> PlayerSnake;

	UPROPERTY(EditAnywhere)
	int32 GridMin = -20;

	UPROPERTY(EditAnywhere)
	int32 GridMax = 20;
	
	static constexpr float GridSize = 100.f;
	
	ESnakeGameState GameState = ESnakeGameState::Playing;
	
	void RestartGame();
	
private:
	void SetGameState(ESnakeGameState NewState);
	
	FTimerHandle MoveTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	TSubclassOf<ASnake> PlayerSnakeClass;
	
	UPROPERTY()
	TObjectPtr<ASnakeItem> ItemFood;
	
	UPROPERTY(EditDefaultsOnly, Category="Food");
	TSubclassOf<ASnakeItem> FoodActorClass;
	
	void SpawnFood();
	bool IsGridOccupied(const FIntPoint& GridPoint) const;
};
