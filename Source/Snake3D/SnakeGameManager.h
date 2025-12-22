// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeGameManager.generated.h"

UENUM(BlueprintType)
enum class ESnakeDirection : uint8
{
	Up,
	Down,
	Left,
	Right
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
	
	UFUNCTION(BlueprintCallable)
	void SetDirection(ESnakeDirection Direction);
	
	UFUNCTION(BlueprintCallable)
	FVector GridToWorld(const FIntPoint& GridPoint) const;
	
	void StepMove();
	
public:
	UPROPERTY(BlueprintReadOnly)
	FIntPoint HeadGrid = FIntPoint(0, 0);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FIntPoint> BodyGrids;
	
	UPROPERTY(BlueprintReadOnly)
	FIntPoint FoodGrid = FIntPoint(0, 0);
	
	UPROPERTY(BlueprintReadOnly)
	ESnakeDirection Direction = ESnakeDirection::Right;

	UPROPERTY(EditAnywhere)
	int32 GridMin = -20;

	UPROPERTY(EditAnywhere)
	int32 GridMax = 20;
	
	UPROPERTY(EditAnywhere)
	float GridSize = 100.f;
	
private:
	FTimerHandle MoveTimer;
};
