// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFood.h"

#include "Snake3D/Snake.h"

// Sets default values
AItemFood::AItemFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Effect = ESnakeEffectType::Grow;
}

void AItemFood::OnConsumed(ASnake* Snake)
{
	Super::OnConsumed(Snake);

	Snake->Grow();
}
