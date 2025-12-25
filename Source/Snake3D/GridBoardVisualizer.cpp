// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBoardVisualizer.h"

// Sets default values
AGridBoardVisualizer::AGridBoardVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridBoardVisualizer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridBoardVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridBoardVisualizer::DrawDebugGrid() const
{
	
}

