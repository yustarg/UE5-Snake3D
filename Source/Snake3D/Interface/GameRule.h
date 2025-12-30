// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRule.generated.h"

class ASnake;
class ASnakeGameManager;

UINTERFACE()
class SNAKE3D_API UGameRule : public UInterface
{
	GENERATED_BODY()
};

class SNAKE3D_API IGameRule
{
	GENERATED_BODY()
	
public:
	// StepMove 开始前（例如修改方向）
	virtual void PreStep(ASnakeGameManager* GM, ASnake* Snake) {}

	// 计算 NextHead 后，判定是否允许
	virtual bool CanMoveTo(ASnakeGameManager* GM, ASnake* Snake, const FIntPoint& TargetGrid) const
	{
		return true;
	}

	// ⭐ 核心：规则可修改目标格子
	// 返回 false = 明确否决这次移动
	virtual bool ModifyTarget(ASnakeGameManager* GM, ASnake* Snake, FIntPoint& InOutTarget) const
	{
		return true;
	}
	
	// 蛇成功移动后
	virtual void PostMove(ASnakeGameManager* GM, ASnake* Snake) {}
};