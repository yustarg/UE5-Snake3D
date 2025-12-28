// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBuff.h"

void USnakeBuff::Initialize(ASnake* InOwner)
{
	Owner = InOwner;
	RemainingTime = Duration;
	OnApply();
}

void USnakeBuff::OnApply()
{
}

void USnakeBuff::OnExpire()
{
}

void USnakeBuff::Tick(const float DeltaTime)
{
	RemainingTime -= DeltaTime;
	if (RemainingTime <= 0.f)
	{
		OnExpire();
	}
}

