// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeFood.h"

#include "Snake3D/Snake.h"

// Sets default values
ASnakeFood::ASnakeFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnakeFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeFood::ApplyTo(ASnake* Snake)
{
	if (!Snake) return;
	Snake->Grow();
}

