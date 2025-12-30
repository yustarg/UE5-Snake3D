// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeAIController.h"

// Sets default values
ASnakeAIController::ASnakeAIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FIntPoint ASnakeAIController::GetNextDirection(const ASnake* Snake)
{
	// 示例：随机一个方向
	static const TArray<FIntPoint> Dirs =
	{
		{1,0}, {-1,0}, {0,1}, {0,-1}
	};

	return Dirs[FMath::RandRange(0, Dirs.Num() - 1)];
}

// Called when the game starts or when spawned
void ASnakeAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

