// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpeedUp.h"

#include "Snake3D/Snake.h"

AItemSpeedUp::AItemSpeedUp()
{
	Effect = ESnakeEffectType::SpeedUp;
}

void AItemSpeedUp::OnEaten(ASnake* Snake)
{
	// Speed Up is a buff now
}
