// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Snake3D/Interface/GameRule.h"
#include "WallWrapRule.generated.h"

UCLASS()
class SNAKE3D_API UWallWrapRule : public UObject, public IGameRule
{
	GENERATED_BODY()
	
public:
	UWallWrapRule();
	
	virtual bool CanMoveTo(ASnakeGameManager* GM,ASnake* Snake, const FIntPoint& TargetGrid) const override;
	
	virtual bool ModifyTarget(ASnakeGameManager* GM, ASnake* Snake, FIntPoint& InOutTarget) const override;
};
