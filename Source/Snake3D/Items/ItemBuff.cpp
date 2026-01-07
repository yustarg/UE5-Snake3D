// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBuff.h"

#include "Snake3D/Snake.h"

void AItemBuff::OnConsumed(ASnake* Snake)
{
	Super::OnConsumed(Snake);
	
	Snake->AddBuff(BuffClass);	
}
