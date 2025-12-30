// Fill out your copyright notice in the Description page of Project Settings.

#include "WallWrapRule.h"

#include "Snake3D/SnakeGameManager.h"
#include "Snake3D/Subsystems/World/SnakeGridSubsystem.h"

UWallWrapRule::UWallWrapRule()
{
}

bool UWallWrapRule::CanMoveTo(ASnakeGameManager* GM, ASnake* Snake, const FIntPoint& TargetGrid) const
{
	USnakeGridSubsystem* Grid = GM->GetGridSubsystem();

	// 允许“越界”，由规则修正位置
	return true;
}

bool UWallWrapRule::ModifyTarget(ASnakeGameManager* GM, ASnake* Snake, FIntPoint& InOutTarget) const
{
	const USnakeGridSubsystem* Grid = GM->GetGridSubsystem();
	
	const FIntPoint Min = Grid->GetGridMin();
	const FIntPoint Max = Grid->GetGridMax();

	// X 方向 wrap
	if (InOutTarget.X <= Min.X) InOutTarget.X = Max.X;
	else if (InOutTarget.X >= Max.X) InOutTarget.X = Min.X;

	// Y 方向 wrap
	if (InOutTarget.Y <= Min.Y) InOutTarget.Y = Max.Y;
	else if (InOutTarget.Y >= Max.Y) InOutTarget.Y = Min.Y;
	
	return true;
}
