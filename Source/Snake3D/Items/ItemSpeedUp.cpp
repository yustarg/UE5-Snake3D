// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpeedUp.h"

#include "Snake3D/Snake.h"

AItemSpeedUp::AItemSpeedUp()
{
	Effect = ESnakeEffectType::SpeedUp;
}

// Speed Up is a buff now
void AItemSpeedUp::OnConsumed(ASnake* Snake)
{
	Super::OnConsumed(Snake);
}
